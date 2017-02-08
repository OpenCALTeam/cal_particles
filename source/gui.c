#include "prova.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <time.h>


struct ModelView{
	GLfloat x_rot;
	GLfloat y_rot;
	GLfloat z_trans;
};

struct ModelView model_view;
time_t start_time, end_time;


void display(void)
{
    CALint i, j, k;
    CALint colore;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();

    glTranslatef(0, 0, model_view.z_trans);
    glRotatef(model_view.x_rot, 1, 0, 0);
    glRotatef(model_view.y_rot, 0, 1, 0);

    // Save the lighting state variables
    glPushAttrib(GL_LIGHTING_BIT);
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glColor3f(0,1,0);
    glScalef(u_modellu->rows, u_modellu->columns, u_modellu->slices);
    //glutWireCube(1.0);
    glPopMatrix();
    // Restore lighting state variables
    glPopAttrib();

    glRotatef(-90,1,0,0);
    for (k=0; k<u_modellu->slices; k++)
        for (i=0; i<u_modellu->rows; i++)
          for (j=0; j<u_modellu->columns; j++)
          {
            colore=PARTICLE_ABSENT;
            for(int w=0;w<MAX_NUMBER_OF_PARTICLES_PER_CELL;w++){
              if(calGet3Di(u_modellu,Q.imove[w],i,j,k)!=PARTICLE_ABSENT)
              {
                colore = calGet3Di(u_modellu,Q.imove[w],i,j,k);
                break;
              }
            }


            if (colore != PARTICLE_ABSENT)
            {
              if(colore == PARTICLE_EDGE){
                glColor3f(0,0,1);
              }
              else{
            	  glColor3f(1,0,0);
              }

              glPushMatrix();
              glTranslated(i-u_modellu->rows/2,j-u_modellu->columns/2,k-u_modellu->slices/2);
              glutSolidSphere(0.5,5,3);
              //glutSolidCube(0.5);
              glPopMatrix();
              //glColor3f(1,0,0);
            }

          }

    glPopMatrix();
    glutSwapBuffers();
}

void simulationRun(void)
{
    CALbyte again;

  //exectutes the global transition function, the steering function and check for the stop condition.
    again = calRunCAStep3D(a_simulazioni);

    //simulation main loop
    a_simulazioni->step++;

    //check for the stop condition
    if (!again)
    {
        //breaking the simulation
        end_time = time(NULL);
        glutIdleFunc(NULL);
        printf("\n");
        printf("Simulation terminated\n");
        printf("Elapsed time: %lds\n", end_time - start_time);

        //graphic rendering
        printf("step: %d; \tactive cells: %d\r", a_simulazioni->step, a_simulazioni->ca3D->A.size_current);
        glutPostRedisplay();
        return;
    }

#ifdef VERBOSE
    //graphic rendering
    printf("step: %d; \tactive cells: %d\r", a_simulazioni->step, a_simulazioni->ca3D->A.size_current);
    glutPostRedisplay();
#endif
}

void init(void)
{
	GLfloat  ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat  diffuseLight[] = { 0.75f, 0.75f, 0.75f, 1.0f };

	glEnable(GL_LIGHTING);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuseLight);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_FLAT);

	glEnable (GL_DEPTH_TEST);

	model_view.x_rot = 0.0;
	model_view.y_rot = 0.0;
	model_view.z_trans = 0.0;

    printf("The 3D particles computational model\n");
	printf("Left click on the graphic window to start the simulation\n");
	printf("Right click on the graphic window to stop the simulation\n");
}

void reshape(int w, int h)
{
	GLfloat	 lightPos[]	= { 0.0f, 0.0f, 100.0f, 1.0f };
    int MAX = u_modellu->rows;

    if (MAX < u_modellu->columns)
        MAX = u_modellu->columns;
    if (MAX < u_modellu->slices)
        MAX = u_modellu->slices;

	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective(45.0, (GLfloat) w/(GLfloat) h, 1.0, 4*MAX);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt (0.0, 0.0, 2*MAX, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	lightPos[2] = 2*MAX;
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
}

void mouse(int button, int state, int x, int y)
{
    switch (button) {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN)
        {
            start_time = time(NULL);
            glutIdleFunc(simulationRun);
        }
        break;
    case GLUT_MIDDLE_BUTTON:
    case GLUT_RIGHT_BUTTON:
        if (state == GLUT_DOWN)
            glutIdleFunc(NULL);
        break;
    default:
        break;
    }
}

void specialKeys(int key, int x, int y){

	GLubyte specialKey = glutGetModifiers();
	const GLfloat x_rot = 5.0, y_rot = 5.0, z_trans = 5.0;

	if(key==GLUT_KEY_DOWN){
		model_view.x_rot += x_rot;
	}
	if(key==GLUT_KEY_UP){
		model_view.x_rot -= x_rot;
	}
	if(key==GLUT_KEY_LEFT){
		model_view.y_rot -= y_rot;
	}
	if(key==GLUT_KEY_RIGHT){
		model_view.y_rot += y_rot;
	}
	if(key == GLUT_KEY_PAGE_UP){
		model_view.z_trans += z_trans;
	}
	if(key == GLUT_KEY_PAGE_DOWN){
		model_view.z_trans -= z_trans;
	}

	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	partilu();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1920, 1080);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(specialKeys);
    glutMouseFunc(mouse);
	glutMainLoop();


	return 0;
}
