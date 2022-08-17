#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <windows.h>
#include <stdbool.h>
#include <math.h>


typedef struct
{

    double x,y,z;

} ponto;


ponto posMouse,posCamera, posCorpo,Sol,Mercurio,Venus,Terra,Lua,Marte,Jupiter,Saturno,Urano,Netuno;


int visaoAtual = 0;
int t = 1;
static bool pause = false;
static bool line_on = false;
static bool camera_travada = true;
static bool light = true;
double tam_planetas[10] = {0.5,0.007,0.017,0.018,0.010,0.198,0.18,0.054,0.052};
char nomes[50][10] = {"Sol","Mercurio","Venus","Terra","Marte","Jupiter","Saturno","Urano","Netuno"};
GLint largura;
GLint altura;

GLuint Fundo_tex,Sol_tex,Cinturao_tex,Venus_tex,Mercurio_tex,Terra_tex,Lua_tex,Marte_tex,Jupiter_tex,Saturno_tex,Anel_Saturno_tex,Urano_tex,Netuno_tex;

GLuint carregaTextura(char* arquivo)
{
    GLuint idTextura = SOIL_load_OGL_texture(
                           arquivo,
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );

    if (idTextura == 0)
    {
        printf("Erro carregando a textura: '%s'\n", SOIL_last_result());
    }

    return idTextura;
}

//função que cria numeros continuos de a cordo com o tempo de funcionamento do programa
double velRot(int vel)
{

    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    double a = t*vel;

    return a;

}

void movimentoMouse(int x, int y)
{
    if(largura!=0 && altura!=0)
    {
        posMouse.x = (x-(largura/2))/(largura/100);
        posMouse.y = (y-(altura/2))/(altura/100);
    }


}

//função para redimensionar a tela de acorod com o tamanho da janela
void redimensiona(int w, int h)
{
    glViewport (0, 0, w, h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)w/(float)h, 1.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);
}

void desenhaOrbita(GLint x, GLint y, GLdouble raio, int num_linhas)
{

    float angulo;
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINE_LOOP);
    glTranslated(0,0,0);
    glRotated(90,0,1,0);
    for(int i = 0; i < num_linhas; i++)
    {
        angulo = i * 2 * M_PI / num_linhas;
        glVertex3f(x + (cos(angulo) * raio), 0,y + (sin(angulo) * raio));
    }

    glEnd();


}

void desenhaDisco(double x,double y,double z,double i,double o,double t,double r,double tam,GLuint text)
{

    GLUquadric* quadObj = gluNewQuadric();

    gluQuadricDrawStyle(quadObj, GLU_FILL);

    gluQuadricNormals(quadObj, GLU_SMOOTH);

    gluQuadricTexture(quadObj, GL_TRUE);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, text);
    glPushMatrix();
    glTranslated(0,0,0);
    glRotated(velRot(t),0,1,0);
    glTranslated(x,y,z);
    glRotated(velRot(r),0,1,0);
    glRotated(-90,1,0,0);
    gluDisk(quadObj,i,o,200,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);




}

//cria so planos
void desenhaPlanos(double x,double y,double z,double t,double r,float rt,int rx,int ry,int rz,int s,GLint text)
{


    glPushMatrix();
    glColor3d(1, 1, 1);
    glTranslated(0,0,0);
    glRotated(velRot(t),0,1,0);
    glTranslatef(x, y, z);
    glRotated(velRot(r),0,1,0);
    glScalef(s, s, s);
    glRotatef(rt, rx, ry, rz);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, text);
    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0, 0);
    glVertex3f(-1, -1,  0);
    glTexCoord2f(1, 0);
    glVertex3f( 1, -1,  0);
    glTexCoord2f(1, 1);
    glVertex3f( 1,  1,  0);
    glTexCoord2f(0, 1);
    glVertex3f(-1,  1,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glFlush();
    glPopMatrix();


}

//para aplicar uma tetura em uma esfera nao se pode usar glSolidSphere
void solidSphere(double radius, int stacks, int columns)
{

    GLUquadric* quadObj = gluNewQuadric();

    gluQuadricDrawStyle(quadObj, GLU_FILL);

    gluQuadricNormals(quadObj, GLU_SMOOTH);

    gluQuadricTexture(quadObj, GL_TRUE);

    gluSphere(quadObj, radius, stacks, columns);

    gluDeleteQuadric(quadObj);
}

//cria uma esfera com movimento
void esfera(double x,double y,double z,double t,double r,double tam,GLuint text)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, text);
    glPushMatrix();
    glTranslatef(z*(cos(velRot(t) / 100)),0,z*(sin(velRot(t) / 100)) );
    glRotated(velRot(r),0,1,0);
    glRotated(-90,1,0,0);
    solidSphere(tam,200,200);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);


}
//cria uma lua com movimento
void luas(double x,double y,double z,double px,double py,double pz,double t,double t2,double r,double tam,GLuint text)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, text);
    glPushMatrix();
    glTranslated(0,0,0);
    glRotated(velRot(t),0,1,0);
    glTranslated(px,py,pz);
    glRotated(velRot(t2),0,1,0);
    glTranslated(x,y,z);
    glRotated(velRot(r),0,1,0);
    glRotated(-90,1,0,0);
    solidSphere(tam,200,200);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);


}

void defineIluminacaoAmbiente (float r, float g, float b)
{

    GLfloat ambient_mode[4]= {r,g,b,1.0};
    GLfloat difuse_light[4]= {r,g,b,1.0};
    GLfloat spec_light[4]= {r, g, b, 1.0};
    GLfloat pos_light[4]= {0.0, 0.0, 0.0, 1.0};

    GLfloat spec[4]= {1.0,1.0,1.0,1.0};
    GLint specMat = 50;


    glMateriali(GL_FRONT,GL_SHININESS,specMat);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_mode);

    glLightfv(GL_LIGHT0, GL_SPECULAR, spec_light );
    glLightfv(GL_LIGHT0, GL_POSITION, pos_light );


    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);





}

void camera(){
if(camera_travada == true)
    {

        glLoadIdentity();
        gluLookAt(posCamera.x,posCamera.y,posCamera.z,
                  posCorpo.x,   posCorpo.y,   posCorpo.z,
                  0,    1,    0     );

    }

    else
    {

        glLoadIdentity();
        gluLookAt(posCamera.x,posCamera.y,posCamera.z,
                  posMouse.x,   0,   posMouse.y,
                  0,    1,    0     );
    }
}

void display()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(light)
    {

        defineIluminacaoAmbiente(1,1, 1);
    }

    if(line_on)
    {
        glColor3f (1, 0.0, 0.0);
        glBegin(GL_LINES);
        glVertex3d(100,0,0);
        glVertex3d(-100,0,0);
        glEnd();
        glBegin(GL_LINES);
        glColor3f (0, 1, 0.0);
        glVertex3d(0,100,0);
        glVertex3d(0,-100,0);
        glEnd();
        glBegin(GL_LINES);
        glColor3f (0, 0, 1);
        glVertex3d(0,0,100);
        glVertex3d(0,0,-100);
        glEnd();
        glFlush();

        desenhaOrbita(0,0,3.16,200);
        desenhaOrbita(0,0,4.16,200);
        desenhaOrbita(0,0,5.0,200);
        desenhaOrbita(0,0,6.6,200);
        desenhaOrbita(0,0,17.0,200);
        desenhaOrbita(0,0,30.6,200);
        desenhaOrbita(0,0,59.4,200);
        desenhaOrbita(0,0,92.0,200);
    }


    //fundo x
    desenhaPlanos(-100,0,0,0,0,90,0,1,0,200,Fundo_tex);

    //fundo x+
    desenhaPlanos(100,0,0,0,0,-90,0,1,0,200,Fundo_tex);

    //fundo y
    desenhaPlanos(0,-100,0,0,0,-90,1,0,0,200,Fundo_tex);

    //fundo z
    desenhaPlanos(0,0,-100,0,0,-90,0,0,1,200,Fundo_tex);


    //Esferas

    glColor3d(1,1,1);

    //Sol
    esfera(Sol.x,Sol.y,Sol.z,0,50,1,Sol_tex);
    // desenhaDisco(Sol.x,Sol.y,Sol.z,7,8,0,50,1,Cinturao_tex);

    //Mercurio 0.007
    esfera(Mercurio.x,Mercurio.y,Mercurio.z,35,75,tam_planetas[t*1],Mercurio_tex);

    //Venus 0.017
    esfera(Venus.x,Venus.y,Venus.z,30,75,tam_planetas[t*2],Venus_tex);

    //Terra 0.018
    esfera(Terra.x,Terra.y,Terra.z,25,50,tam_planetas[t*3],Terra_tex);
    //luas(Lua.x,Lua.y,Lua.z,Terra.x,Terra.y,Terra.z,25,30,50,tam_planetas[t*3],Lua_tex);


    //Marte 0.01
    esfera(Marte.x,Marte.y,Marte.z,20,57,tam_planetas[t*4],Marte_tex);

    //Jupiter 0.198
    esfera(Jupiter.x,Jupiter.y,Jupiter.z,15,75,tam_planetas[t*5],Jupiter_tex);

    //Saturno 0.18
    esfera(Saturno.x,Saturno.y,Saturno.z,10,50,tam_planetas[t*6],Saturno_tex);
    desenhaDisco(Saturno.x,Saturno.y,Saturno.z,0.20,0.30,10,50,tam_planetas[t*6],Anel_Saturno_tex);

    //Urano 0.054
    esfera(Urano.x,Urano.y,Urano.z,5,50,tam_planetas[t*7],Urano_tex);

    //Netuno 0.052
    esfera(Netuno.x,Netuno.y,Netuno.z,2,50,tam_planetas[t*8],Netuno_tex);


    camera();


    glutSwapBuffers();

}





void key(unsigned char key, int x, int y)
{

    switch (key)
    {
    case 27 :
        exit(0);
        break;
    //zoom -
    case 's':
        case'S':
        if (visaoAtual == 0 && posCamera.x <=98&&posCamera.y <=98&&posCamera.z <=98)
        {
            posCamera.x += 1;
            posCamera.y += 1;
            posCamera.z += 1;


        }
        else if(visaoAtual == 1 && posCamera.y <=120)
        {
            posCamera.y += 1;

        }

        break;

    //zoom +
    case 'w':
        case'W':
        if  (visaoAtual == 0&&posCamera.x >=3 && posCamera.y >=3 && posCamera.z >=3)
        {
            posCamera.x -= 1;
            posCamera.y -= 1;
            posCamera.z -= 1;

        }
        else if(visaoAtual == 1 || posCamera.y >=3)
        {
            posCamera.y -= 1;

        }
        break;



//mudar o estado de visao
    case ' ':
        if  (visaoAtual == 0)
        {
            posCamera.x = 0.01;
            posCamera.y = 50;
            posCamera.z = 0;
            visaoAtual = 1;
        }
        else
        {
            posCamera.x = 30.0;
            posCamera.y = 30.0;
            posCamera.z = 30.0;
            visaoAtual = 0;
        }
        break;
    //ligar e desligar as linhas do plano cartesiano
    case'l':
    case'L':
        if(line_on == false)
        {
            line_on = true;
        }

        else
        {
            line_on = false;
        }
        break;

    //travar/destravar a camera
    case 'c':
    case 'C':
        if(camera_travada == true)
        {
            camera_travada = false;
        }

        else
        {
            camera_travada = true;
        }
        break;


    //aumentar o tamanho dos planetas
    case 't':
    case 'T':
        if(t == 0)
        {
            t = 1.0;
        }

        else
        {
            t = 0;
        }
        break;
    default:
        break;

    }

}



void atualizaCena(int periodo)
{
    camera();
    glutTimerFunc(periodo, atualizaCena, periodo);
    altura = glutGet(GLUT_WINDOW_HEIGHT);
    largura = glutGet(GLUT_WINDOW_WIDTH);
    glutPostRedisplay();
}


void inicializa()
{

    posCamera.x = 30.0;
    posCamera.y = 30.0;
    posCamera.z = 30.0;
    posMouse.x = 0.0;
    posMouse.y = 0.0;
    posMouse.z = 0.0;
    posCorpo.x = Sol.x;
    posCorpo.y = Sol.y;
    posCorpo.z = Sol.z;

    Sol.x = 0,Sol.y = 0,Sol.z = 0,
        Mercurio.x = 0,Mercurio.y = 0,Mercurio.z = -3.16,
                 Venus.x = 0,Venus.y = 0,Venus.z = -4.16,
                       Terra.x = 0,Terra.y = 0,Terra.z = -5,
                             Lua.x = 0,Lua.y = 0,Lua.z = -0.3,
                                 Marte.x = 0,Marte.y = 0,Marte.z = -6.6;
    Jupiter.x = 0,Jupiter.y = 0,Jupiter.z = -17.0;
    Saturno.x = 0,Saturno.y = 0,Saturno.z = -30.6;
    Urano.x = 0,Urano.y = 0,Urano.z = -59.4;
    Netuno.x = 0,Netuno.y = 0,Netuno.z = -92;



    Fundo_tex = carregaTextura("fundo.jpg");
    Sol_tex = carregaTextura("sol.jpg");
    Venus_tex = carregaTextura("venus.jpg");
    Mercurio_tex = carregaTextura("mercurio.jpg");
    Terra_tex = carregaTextura("terra.jpg");
    Lua_tex = carregaTextura("lua.jpg");
    Marte_tex = carregaTextura("marte.jpg");
    Jupiter_tex = carregaTextura("jupiter.jpg");
    Saturno_tex = carregaTextura("saturno.jpg");
    Urano_tex = carregaTextura("urano.jpg");
    Netuno_tex = carregaTextura("netuno.jpg");
    Anel_Saturno_tex = carregaTextura("anel_saturno.png");
    Cinturao_tex = carregaTextura("cinturao.png");


}


int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(1920,1080);
    glutInitWindowPosition(0,0);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    altura = glutGet(GLUT_WINDOW_HEIGHT);
    largura = glutGet(GLUT_WINDOW_WIDTH);


    glutCreateWindow("Sistema Planetario");
    inicializa();
    glutReshapeFunc(redimensiona);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutPassiveMotionFunc(movimentoMouse);
    glutTimerFunc(0, atualizaCena, 33);

    glClearColor(0,0,0,0);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);



    glutMainLoop();



    return EXIT_SUCCESS;
}
