//Computer Graphics HW-SWGL 2025
//simple modeling tool
//Transformation and Projection
//move from glut to glfw

#include<iostream>

#include <GL/glu.h>
#include <GLFW/glfw3.h>

//OpenGL Mathematics (GLM)  https://glm.g-truc.net/
#include<glm/glm.hpp>
#include<glm/vec3.hpp>
#include<glm/vec4.hpp>
#include<glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <fstream>

using namespace std;
using namespace glm;

const bool STEP2 = true;
const bool STEP3 = true;

float theta = 3.14159f / 4.0f;
float tho = 3.14159f / 4.0f;

int winWidth = 1280;
int winHeight = 720;

mat4x4 transformMat = mat4x4(1);

mat4x4 ViewMat = mat4x4(1);
mat4x4 ProjectionMat = mat4x4(1);

vec3 default_tetrahedron_vertices[4] = {
	vec3(1,0,0), vec3(0,1,0), vec3(0,0,1), vec3(0,0,0)
};
vec3 tetrahedron_verts[4];
vector<vec3> current_vertices;

//step1: implement Translate Matrix
mat4x4 swTranslate(float x, float y, float z)
{
	mat4x4 Translate = mat4x4(1);
	Translate[3][0] = x;
	Translate[3][1] = y;
    Translate[3][2] = z;

	return Translate;
}

//step1: implement Rotate Matrix
mat4x4 swRotateX(float angle)
{
	mat4x4 Rotate = mat4x4(1);
    float rad = radians(angle);
    float cosA = cos(rad);
    float sinA = sin(rad);

    Rotate[1][1] = cosA;
    Rotate[1][2] = -sinA;
    Rotate[2][1] = sinA;
    Rotate[2][2] = cosA;

	return Rotate;
}

mat4x4 swRotateY(float angle)
{
	mat4x4 Rotate = mat4x4(1);
    float rad = radians(angle);
    float cosA = cos(rad);
    float sinA = sin(rad);

    Rotate[0][0] = cosA;
    Rotate[0][2] = sinA;
    Rotate[2][0] = -sinA;
    Rotate[2][2] = cosA;
	return Rotate;
}

mat4x4 swRotateZ(float angle)
{
	mat4x4 Rotate = mat4x4(1);
    float rad = radians(angle);
    float cosA = cos(rad);
    float sinA = sin(rad);

    Rotate[0][0] = cosA;
    Rotate[0][1] = -sinA;
    Rotate[1][0] = sinA;
    Rotate[1][1] = cosA;
	return Rotate;
}

//optinal
mat4x4 swRotate(float angle,float x, float y, float z)
{
	mat4x4 Rotate = mat4x4(1);
    vec3 axis = normalize(vec3(x, y, z));
    float rad = radians(angle);
    float cosA = cos(rad);
    float sinA = sin(rad);

    Rotate[0][0] = cosA + axis.x * axis.x * (1 - cosA);
    Rotate[0][1] = axis.x * axis.y * (1 - cosA) - axis.z * sinA;
    Rotate[0][2] = axis.x * axis.z * (1 - cosA) + axis.y * sinA;

    Rotate[1][0] = axis.y * axis.x * (1 - cosA) + axis.z * sinA;
    Rotate[1][1] = cosA + axis.y * axis.y * (1 - cosA);
    Rotate[1][2] = axis.y * axis.z * (1 - cosA) - axis.x * sinA;

    Rotate[2][0] = axis.z * axis.x * (1 - cosA) - axis.y * sinA;
    Rotate[2][1] = axis.z * axis.y * (1 - cosA) + axis.x * sinA;
    Rotate[2][2] = cosA + axis.z * axis.z * (1 - cosA);
	return Rotate;
}

//step1: implement Scale(x, y, z)
mat4x4 swScale(float x, float y, float z)
{
	mat4x4 Scale = mat4x4(1);
    Scale[0][0] = x;
    Scale[1][1] = y;
    Scale[2][2] = z;
	return Scale;
}

//step2: 
void swLookAt(	float eyeX, float eyeY, float eyeZ,
    float centerX, float centerY, float centerZ,
    float upX, float upY, float upZ,
    mat4 &View) // 
{
    vec3 eye = vec3(eyeX, eyeY, eyeZ);
    vec3 center = vec3(centerX, centerY, centerZ);
    vec3 up = vec3(upX, upY, upZ);

    vec3 f = normalize(center - eye);
    vec3 r = normalize(cross(f, normalize(up)));
    vec3 u = cross(r, f);

    View = mat4(1.0f);

    View[0][0] = r.x;
    View[1][0] = r.y;
    View[2][0] = r.z;

    View[0][1] = u.x;
    View[1][1] = u.y;
    View[2][1] = u.z;

    View[0][2] = -f.x;
    View[1][2] = -f.y;
    View[2][2] = -f.z;

    View[3][0] = -dot(r, eye);
    View[3][1] = -dot(u, eye);
    View[3][2] = dot(f, eye);
}

void swPerspective(float fov, float aspect, float near, float far, mat4 &Projection)
{
    Projection = mat4(1.0f);

    float tanHalfFov = tan(radians(fov) / 2.0f);

    Projection[0][0] = 1.0f / (aspect * tanHalfFov);
    Projection[1][1] = 1.0f / tanHalfFov;
    Projection[2][2] = -(far + near) / (far - near);
    Projection[2][3] = -1.0f;
    Projection[3][2] = -(2.0f * far * near) / (far - near);
}

//step3: implemet 


void swTriangle(vec3 color, vec3 in_v1, vec3 in_v2, vec3 in_v3, mat4x4 Modelmatrix)
{
	vec4 v1(in_v1.x, in_v1.y, in_v1.z, 1);
	vec4 v2(in_v2.x, in_v2.y, in_v2.z, 1);
	vec4 v3(in_v3.x, in_v3.y, in_v3.z, 1);

	//step1
    v1 = Modelmatrix * v1;
	v2 = Modelmatrix * v2;
	v3 = Modelmatrix * v3;

	//step2: remove glLookAt, compute view matrix
	v1 = ViewMat * v1;
    v2 = ViewMat * v2;
    v3 = ViewMat * v3;


	//step3: remove glProjection, compute project matrix
	v1 = ProjectionMat * v1;
	v2 = ProjectionMat * v2;
	v3 = ProjectionMat * v3;

	// prespective division
    if (v1.w != 0) v1 /= v1.w;
    if (v2.w != 0) v2 /= v2.w;
    if (v3.w != 0) v3 /= v3.w;

	glColor3f(color.r, color.g, color.b);
	glVertex3f(v1.x, v1.y, v1.z);
	glVertex3f(v2.x, v2.y, v2.z);
	glVertex3f(v3.x, v3.y, v3.z);
}



void Draw_Tetrahedron() {
	vec3 color(1, 1, 0);
	//glColor3f(1, 1, 0);
	glBegin(GL_TRIANGLES);

		swTriangle(vec3(1, 0, 0), tetrahedron_verts[0], tetrahedron_verts[1], tetrahedron_verts[2], transformMat);

		swTriangle(vec3(0, 0, 1), tetrahedron_verts[3], tetrahedron_verts[0], tetrahedron_verts[1], transformMat);

		swTriangle(vec3(0, 1, 0), tetrahedron_verts[2], tetrahedron_verts[3], tetrahedron_verts[0], transformMat);

		swTriangle(vec3(1, 1, 0), tetrahedron_verts[1], tetrahedron_verts[2], tetrahedron_verts[3], transformMat);

	glEnd();
}

void DrawGrid(int size = 10)
{
	glBegin(GL_LINES);
	glColor3f(0.3, 0.3, 0.3);
	for (int i = 1; i < size; i++) {
		glVertex3f(i, -size, 0);
		glVertex3f(i, size, 0);
		glVertex3f(-i, -size, 0);
		glVertex3f(-i, size, 0);
		glVertex3f(-size, i, 0);
		glVertex3f(size, i, 0);
		glVertex3f(-size, -i, 0);
		glVertex3f(size, -i, 0);
	}
	glEnd();

	glBegin(GL_LINES);
		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(size, 0, 0);
		glColor3f(0.4, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(-size, 0, 0);

		glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, size, 0);
		glColor3f(0, 0.4, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, -size, 0);

		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, size);
	glEnd();
}

void Display(GLFWwindow* window)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 1, 0.1, 50);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(10 * cos(theta), -10 * sin(theta), 10, 0, 0, 0, 0, 0, 1);

	DrawGrid();

	//step 3: PROJECTION
	if (STEP3 == true) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//glOrtho(0, winWidth, 0, winHeight, -2.0, 2.0);
		ProjectionMat = mat4x4(1);
		swPerspective(60, 1, 0.1, 50,ProjectionMat);

	} 

	//step 2: viewing 
	if (STEP2 == true) {
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		ViewMat = mat4x4(1);
		swLookAt(10 * cos(theta), -10 * sin(theta), 10, 0, 0, 0, 0, 0, 1,ViewMat);

	} 

	Draw_Tetrahedron();

	glFlush();
	glfwSwapBuffers(window);
}

void init() {
	glClearColor(0, 0, 0, 0);
	glClearDepth(1.0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}



// Converted special key function for GLFW.
void SpecialKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // Process only key press and repeated actions.
    if (action != GLFW_PRESS && action != GLFW_REPEAT)
        return;
    
    switch (key) {
        case GLFW_KEY_F1:
            glfwSetWindowTitle(window, "F1: add a tetrahedron");
            for (int i = 0; i < 4; i++) {
                tetrahedron_verts[i][0] = default_tetrahedron_vertices[i][0];
                tetrahedron_verts[i][1] = default_tetrahedron_vertices[i][1];
                tetrahedron_verts[i][2] = default_tetrahedron_vertices[i][2];
            }
            break;
            
        case GLFW_KEY_F2:
            glfwSetWindowTitle(window, "F2: add a cube or somthing");

            break;
            
        case GLFW_KEY_F5:
            glfwSetWindowTitle(window, "F5: SAVE");
        
            break;
            
        case GLFW_KEY_F6:
            glfwSetWindowTitle(window, "F6: LOAD");
         
            break;
            
        default:
            break;
    }
}


void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // Here you can handle both regular and special keys.
    // Call your special key function for function keys.
    if (key == GLFW_KEY_F1 || key == GLFW_KEY_F2 ||
        key == GLFW_KEY_F5 || key == GLFW_KEY_F6) {
        SpecialKey(window, key, scancode, action, mods);
    }
    
    // Add handling for other keys as needed.

    // Only handle press and repeat events.
    if (action != GLFW_PRESS && action != GLFW_REPEAT)
        return;

    switch (key) {
        case GLFW_KEY_ESCAPE:  // ESC key
            exit(0);
            break;

        // rotate world
        case GLFW_KEY_9:
            theta += 3.14159f / 90.0f;
            break;
        case GLFW_KEY_0:
            theta -= 3.14159f / 90.0f;
            break;
        case GLFW_KEY_MINUS:
            transformMat = mat4x4(1);
            break;

        // translate +x (handles both 'd' and 'D')
        case GLFW_KEY_D:
            glfwSetWindowTitle(window, "translate +x");
            transformMat = swTranslate(1, 0, 0) * transformMat;
            break;

        // translate -x (handles both 'a' and 'A')
        case GLFW_KEY_A:
            glfwSetWindowTitle(window, "translate -x");
            transformMat = swTranslate(-1, 0, 0) * transformMat;
            break;

        // translate +y (handles both 'w' and 'W')
        case GLFW_KEY_W:
            glfwSetWindowTitle(window, "translate +y");
            transformMat = swTranslate(0, 1, 0) * transformMat;
            break;
        
        // translate -y (handles both 's' and 'S')
        case GLFW_KEY_S:
            glfwSetWindowTitle(window, "translate -y");
            transformMat = swTranslate(0, -1, 0) * transformMat;
            break;

        // rotate +x (handles both 'h' and 'H')
        case GLFW_KEY_H:
            glfwSetWindowTitle(window, "rotate +x");
            transformMat = swRotateX(10) * transformMat;
            break;
        // rotate -x (handles both 'f' and 'F')
        case GLFW_KEY_F:
            glfwSetWindowTitle(window, "rotate -x");
            transformMat = swRotateX(-10) * transformMat;
            break;
        // rotate +y (handles both 't' and 'T')
        case GLFW_KEY_T:
            glfwSetWindowTitle(window, "rotate +Y");
            transformMat = swRotateY(10) * transformMat;
            break;
        // rotate -y (handles both 'g' and 'G')
        case GLFW_KEY_G:
            glfwSetWindowTitle(window, "rotate -x");
            transformMat = swRotateY(-10) * transformMat;
            break;
        
        // scale +x (handles both 'l' and 'L')
        case GLFW_KEY_L:
        glfwSetWindowTitle(window, "scale +x");
        transformMat = swScale(1.1,1,1) * transformMat;
        break;
        // scale -x (handles both 'j' and 'J')
        case GLFW_KEY_J:
        glfwSetWindowTitle(window, "scale -x");
        transformMat = swScale(-1.1,1,1) * transformMat;
        break;
        // scale +y (handles both 'i' and 'I')
        case GLFW_KEY_I:
        glfwSetWindowTitle(window, "scale +y");
        transformMat = swScale(1,1.1,1)* transformMat;
        break;
        // scale -y (handles both 'k' and 'K')
        case GLFW_KEY_K:
        glfwSetWindowTitle(window, "scale -y");
        transformMat = swScale(1,-1.1,1) * transformMat;
        break;
        // scale(small) x (handles both 'o' and 'O')
        case GLFW_KEY_O:
        glfwSetWindowTitle(window, "scale(small) x");
        transformMat = swScale(0.9,1,1) * transformMat;
        break;// scale(small) y  (handles both 'u' and 'U')
        case GLFW_KEY_U:
        glfwSetWindowTitle(window, "scale(small) y");
        transformMat = swScale(1,0.9,1) * transformMat;
        break;


        // Add other keys as needed.
        default:
            break;
    }
}


int main(void)
{
    // Initialize GLFW
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(winWidth, winHeight, "trans: Press F1 to add a tetrahedron", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Set callback for keyboard events
    glfwSetKeyCallback(window, KeyCallback);

    // Initialize OpenGL
    init();

    // Timing for periodic updates (~33ms interval)
    double previousTime = glfwGetTime();
    const double interval = 0.033; // ~33ms

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Check if it's time to update (simulate timer)
        double currentTime = glfwGetTime();
        if (currentTime - previousTime >= interval) {
            // Here you can include any periodic update logic.
            // In GLUT you used glutPostRedisplay(), but in GLFW,
            // since you're in control of the loop, just call Display().
            previousTime = currentTime;
        }

        // Render here
        Display(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Clean up and exit
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}