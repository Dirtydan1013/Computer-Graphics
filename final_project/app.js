// app.js
import * as THREE        from 'three';
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls.js';
import { GUI }           from 'dat.gui';

let scene, camera, renderer, controls;
let geometry, material, mesh;
let heightData, streamData;
let raycaster, mouse;
let params, gui;
let brushActive = false, lastCenter = null;

init();
animate();

function init() {
  // —— 场景 / 相机 / 渲染器
  scene    = new THREE.Scene();
  camera   = new THREE.PerspectiveCamera(60, window.innerWidth/window.innerHeight, 1, 5000);
  camera.position.set(0, -300, 300);
  renderer = new THREE.WebGLRenderer({antialias:true});
  renderer.setSize(window.innerWidth, window.innerHeight);
  document.body.appendChild(renderer.domElement);

  // —— 网格 & 材质
  const seg = 200;
  geometry = new THREE.PlaneGeometry(400, 400, seg, seg);
  geometry.rotateX(-Math.PI/2);
  material = new THREE.MeshStandardMaterial({
    color:      0x88aa88,
    side:       THREE.DoubleSide,
    wireframe:  false
  });
  mesh = new THREE.Mesh(geometry, material);
  scene.add(mesh);

  // —— 初始化数据
  const N    = seg + 1;
  const cell = 400/seg;
  const diag = Math.hypot(cell, cell);
  heightData = new Float32Array(N*N).fill(0);
  streamData = new Float32Array(N*N).fill(diag);

  updateGeometry();

  // —— 光照
  const dir = new THREE.DirectionalLight(0xffffff,1);
  dir.position.set(1,1,1);
  scene.add(dir);
  scene.add(new THREE.AmbientLight(0x666666));

  // —— 控制 & 拾取
  controls  = new OrbitControls(camera, renderer.domElement);
  raycaster = new THREE.Raycaster();
  mouse     = new THREE.Vector2();

  // —— 画笔 (Ctrl + 左键)
  renderer.domElement.addEventListener('pointerdown', e => {
    if (e.ctrlKey && e.button===0) {
      brushActive = true;
      controls.enabled = false;
      pick(e);
    }
  }, { capture:true });
  renderer.domElement.addEventListener('pointermove', e => {
    if (brushActive) pick(e);
  });
  renderer.domElement.addEventListener('pointerup', e => {
    if (e.button===0) {
      brushActive = false;
      controls.enabled = true;
    }
  });

  // —— 参数 & GUI
  params = {
    brushRadius:      10,
    brushStrength:    5,
    wireframe:        false,
    Simulation:       false,
    U:                0.002,    // 抬升速率
    K:                0.0005,   // 流蚀系数
    p_sa:             0.8,      // 面积指数
    p_sl:             2.0,      // 坡度指数
    dt:               1.0,      // 步长
    simStepsPerFrame: 2         // 每帧跑几次 simulateStep()
  };
  gui = new GUI();
  gui.add(params,'wireframe').name('Wireframe').onChange(v=>material.wireframe=v);
  gui.add(params,'Simulation').name('Simulation');
  const f = gui.addFolder('Erosion');
  f.add(params,'U',0.0001,0.01,0.0001).name('U (uplift)');
  f.add(params,'K',0.0001,0.005,0.0001).name('K (erodibility)');
  f.add(params,'p_sa',0.1,1.5,0.1).name('m (area)');
  f.add(params,'p_sl',1,4,0.1).name('n (slope)');
  f.add(params,'dt',0.01,5,0.01).name('Δt');
  f.add(params,'simStepsPerFrame',1,10,1).name('Steps/Frame');
  f.open();

  window.addEventListener('resize', onWindowResize);
}

function pick(e) {
  mouse.x = (e.clientX / window.innerWidth)*2 - 1;
  mouse.y = - (e.clientY / window.innerHeight)*2 + 1;
  raycaster.setFromCamera(mouse, camera);
  const hits = raycaster.intersectObject(mesh);
  if (hits.length) {
    lastCenter = hits[0].point.clone();
    applyBrush(lastCenter);
    updateGeometry();
  }
}

function updateGeometry() {
  const pos = geometry.attributes.position.array;
  const seg = Math.sqrt(pos.length/3) - 1;
  let idx = 0;
  for (let j = 0; j <= seg; j++) {
    for (let i = 0; i <= seg; i++) {
      pos[3*idx + 1] = heightData[j*(seg+1) + i];
      idx++;
    }
  }
  geometry.attributes.position.needsUpdate = true;
  geometry.computeVertexNormals();
}

function applyBrush(center) {
  const seg = Math.sqrt(heightData.length) - 1;
  const half = 200;
  for (let j = 0; j <= seg; j++) {
    for (let i = 0; i <= seg; i++) {
      const x = -half + (i/seg)*400;
      const z = -half + (j/seg)*400;
      const d = Math.hypot(x - center.x, z - center.z);
      if (d < params.brushRadius) {
        heightData[j*(seg+1) + i] += (1 - d/params.brushRadius) * params.brushStrength;
      }
    }
  }
}

const offs = [
  [0,1],[1,1],[1,0],[1,-1],
  [0,-1],[-1,-1],[-1,0],[-1,1]
];

function simulateStep() {
  const seg = Math.sqrt(heightData.length) - 1;
  const N   = seg + 1;
  const cell = 400/seg;
  const diag = Math.hypot(cell, cell);
  const { U, K, p_sa, p_sl, dt } = params;

  // 1) 水量累積 (一次 WaterSteepest)
  const newStream = new Float32Array(N*N);
  for (let j = 0; j < N; j++) {
    for (let i = 0; i < N; i++) {
      const idx = j*N + i;
      if (i===0||j===0||i===seg||j===seg) {
        newStream[idx] = diag;
        continue;
      }
      let waterIn = 0;
      for (let k = 0; k < 8; k++) {
        const ni = i + offs[k][0], nj = j + offs[k][1];
        if (ni<0||nj<0||ni>seg||nj>seg) continue;
        // 鄰近最陡方向
        let bestSl = 0, dx = 0, dy = 0;
        const nIdx = nj*N + ni;
        for (let m = 0; m < 8; m++) {
          const mi = ni + offs[m][0], mj = nj + offs[m][1];
          if (mi<0||mj<0||mi>seg||mj>seg) continue;
          const dist = Math.hypot(offs[m][0]*cell, offs[m][1]*cell);
          const sl = (heightData[nIdx] - heightData[mj*N+mi]) / dist;
          if (sl > bestSl) { bestSl = sl; dx = offs[m][0]; dy = offs[m][1]; }
        }
        if (ni+dx===i && nj+dy===j) {
          waterIn += streamData[nIdx];
        }
      }
      newStream[idx] = diag + waterIn;
    }
  }
  streamData = newStream;

  // 2) Stream‐power + Uplift
  const newH = new Float32Array(N*N);
  for (let j = 0; j < N; j++) {
    for (let i = 0; i < N; i++) {
      const idx = j*N + i;
      if (i===0||j===0||i===seg||j===seg) {
        newH[idx] = 0;
        continue;
      }
      // 最陡下游坡度
      let bestSl = 0, dx = 0, dy = 0;
      for (let k = 0; k < 8; k++) {
        const ni = i + offs[k][0], nj = j + offs[k][1];
        if (ni<0||nj<0||ni>seg||nj>seg) continue;
        const dist = Math.hypot(offs[k][0]*cell, offs[k][1]*cell);
        const sl = (heightData[idx] - heightData[nj*N+ni]) / dist;
        if (sl > bestSl) { bestSl = sl; dx = offs[k][0]; dy = offs[k][1]; }
      }
      const A   = streamData[idx];
      const spe = K * Math.pow(A, p_sa) * Math.pow(bestSl, p_sl);
      let hNew  = heightData[idx] - dt*spe;
      const rH  = heightData[(j+dy)*N + (i+dx)];
      if (hNew < rH) hNew = rH;
      hNew += dt*U;
      newH[idx] = hNew;
    }
  }
  heightData = newH;
}

function animate() {
  requestAnimationFrame(animate);

  // 如果开 Simulation，每帧做若干步
  if (params.Simulation) {
    for (let i = 0; i < params.simStepsPerFrame; i++) {
      simulateStep();
    }
    updateGeometry();
  }

  controls.update();
  renderer.render(scene, camera);
}

function onWindowResize() {
  camera.aspect = window.innerWidth/window.innerHeight;
  camera.updateProjectionMatrix();
  renderer.setSize(window.innerWidth, window.innerHeight);
}
