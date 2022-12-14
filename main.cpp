
//*****************************************************
//
// Exemplo3DComZoomEPan.cpp
// Um programa OpenGL simples que abre uma janela GLUT,
// desenha um "torus" e permite fazer: zoom in e
// zoom out quando os bot�es do mouse s�o pressionados
// (utiliza a proje��o perspectiva); e pan quando as
// teclas page up, page down, seta para cima, seta para
// baixo, seta para esquerda e seta para direita s�o
// pressionadas.
//
// Marcelo Cohen e Isabel H. Manssour
// Este c�digo acompanha o livro
// "OpenGL - Uma Abordagem Pr�tica e Objet

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

GLfloat angle, fAspect;
GLfloat deslocamentoX, deslocamentoY, deslocamentoZ;
GLfloat lookx, looky, deslocamentoZZ;
GLfloat angleRotate = 0;

static int slices = 20;
static int stacks = 20;

// Planetas:                Venus | Mercurio | Terra | Marte | Júpiter | Saturno | Urano | Netuno

// Raios dos Planetas
double raios[8] = {1, 1.5, 2, 1.75, 2.1, 2.2, 2, 1.1};

// Velociodade de Translação dos Planetas
double velocidadesT[8] = {1.8, 0.64, 0.44, 0.23, 13, 5.46, 1.95, 1};

// Velocidade de Rotação dos Planetas
double velocidadesR[8] = {0.016, 0.004, 1, 0.97, 2.4, 2.34, 1.41, 1.5};

// Especificação das cores em RGB dos Planetas
double rs[8] = {1.0, 0.4, 0.0, 1.0, 1.0, 0.8, 0.4, 0.8};
double gs[8] = {0.5, 0.2, 0.5, 0.0, 0.8, 0.5, 0.7, 0.8};
double bs[8] = {0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.2, 1.0};

// Especificação da quantidade de aneis e satelites de cada planeta
int aneis[8] = {0, 0, 0, 1, 1, 4, 2, 1};
int satelites[8] = {0, 0, 1, 2, 1, 1, 1, 2};

// Especificacao dos raios da elipse (Orbitas)
int raioMaior = 2;
int raioMenor = 1;

//---------------------------------------------- Control ---------------------------------------------------------
// Angulos dos Satelites e planetas
double angsSatelite[8] = {0, 0, 0, 0, 0, 0, 0, 0};
double angsPlaneta[8] = {0, 0, 0, 0, 0, 0, 0, 0};

// Exibicao de cada Planeta
bool show[8] = {true, true, true, true, true, true, true, true};

// Exibicao de todas as Orbitas
bool showOrbita = true;

// Ativar/Desativar efeito de animacao do sistema
bool animate = true;

// Calaculo e exibicao do tempo de transalacao da Terra
int tempoTerra = 0;
bool showedTempoTerra = false;

// Funcao para desenhar linhas
void desenhaTerreno()
{
    float L = 500.0;
    float incr = 10.0;
    float y = 0.2;
    glColor3f(1.0f, 1.0f, 1.0f);

    for (float i = -L; i <= L; i += incr)
    {
        glBegin(GL_LINES);
        // Verticais
        glVertex3f(i, -L, y);
        glVertex3f(i, L, y);

        // Horizontais
        glVertex3f(-L, i, y);
        glVertex3f(L, i, y);
        glEnd();
    }

}

// Função callback chamada para fazer o desenho da orbita
void DesenhaOrbita(float raio)
{

    float angulo, incremento;
    int vertices = 100;

    // Especifica que a cor corrente é branca RGB
    glColor3f(1.0f, 1.0f, 1.0f);

    // Especifica o tamanho do ponto
    glPointSize(4.0f);

    incremento = (2 * M_PI) / vertices;

    // Desenha um círculo de linhas
    glBegin(GL_LINES);
    for (angulo = 0; angulo < 2 * M_PI; angulo += incremento)
    {
        glVertex3f(raioMaior * raio * cos(angulo), raioMenor * raio * sin(angulo), 0);
        glVertex3d(raioMaior * raio * cos(angulo + incremento), raioMenor * raio * sin(angulo + incremento), 0);
    }
    glEnd();
}

// Funcao callback que desenha circunferencias estaticas
void DesenhaEstatico(float r, float g, float b, float raio)
{

    float angulo, incremento;
    int vertices = 50;

    // Especifica que a cor RGB
    glColor3f(r, g, b);

    /*
        // Especifica o tamanho do ponto
        glPointSize(1.0f);

        incremento = (2 * M_PI) / vertices;

        glBegin(GL_POLYGON);
        for(angulo=0; angulo<2*M_PI; angulo+=incremento){
            glVertex2f(raio*cos(angulo),raio*sin(angulo));
            glVertex2d(raio*cos(angulo+incremento), raio*sin(angulo+incremento));
        }
        glEnd();
    */

    glutSolidSphere(raio, slices, stacks);
}

// Função callback chamada para fazer o desenho do planeta
// E preciso informar o planeta e o raio da orbita que se encontra
void DesenhaPlaneta(int planeta, float raioOrbita)
{

    // Resgatando as propriedas do planeta e orbita referente
    float raio = raios[planeta];
    float r = rs[planeta];
    float g = gs[planeta];
    float b = bs[planeta];
    int qtdAnel = aneis[planeta];
    int qtdSatelite = satelites[planeta];

    float angulo, incremento;
    int vertices = 50;

    // Especifica que a cor RGB
    glColor3f(r, g, b);

    // Especifica o tamanho do ponto
    /*
        glPointSize(2.0f);

        incremento = (2 * M_PI) / vertices;


        glBegin(GL_POLYGON);
        for(angulo=0; angulo<2*M_PI; angulo+=incremento){
            glVertex2f(raio*cos(angulo),raio*sin(angulo));
            glVertex2d(raio*cos(angulo+incremento), raio*sin(angulo+incremento));
        }
        glEnd();
    */

    glutSolidSphere(raio, slices, stacks);

    for (int anel = 1; anel <= qtdAnel; anel++)
    {
        DesenhaOrbita(raio + anel * 0.5);
    }

    switch (qtdSatelite)
    {
    case 1:
    {
        glPushMatrix();
        // glRotatef(angSatelite[planeta],0.0f,0.0f,1.0f);
        glTranslated(raioMaior * (raio + 0.5) * cos(angsSatelite[planeta]), raioMenor * (raio + 0.5) * sin(angsSatelite[planeta]), 0);
        DesenhaEstatico(0.8, 0.8, 0.8, 0.5);
        glPopMatrix();
        break;
    }
    case 2:
    {
        glPushMatrix();
        // glRotatef(angSatelite[planeta],0.0f,0.0f,1.0f);
        glTranslated(raioMaior * (raio + 0.5) * cos(angsSatelite[planeta]), raioMenor * (raio + 0.5) * sin(angsSatelite[planeta]), 0);
        DesenhaEstatico(0.8, 0.8, 0.8, 0.5);
        glPopMatrix();
        glPushMatrix();
        // glRotatef(angSatelite[planeta],0.0f,0.0f,1.0f);
        glTranslated(-raioMaior * (raio + 0.5) * cos(angsSatelite[planeta]), -raioMenor * (raio + 0.5) * sin(angsSatelite[planeta]), 0);
        DesenhaEstatico(0.8, 0.8, 0.8, 0.5);
        glPopMatrix();
        break;
    }
    }
}

void DefineIluminacao()
{
    // Luz 0
    GLfloat luzAmbiente[4] = {0.05, 0.05, 0.05, 1.0};
    GLfloat luzDifusa[4] = {0.5, 0.5, 0.5, 1.0};    // "cor"
    GLfloat luzEspecular[4] = {0.1, 0.1, 0.1, 1.0}; // "brilho"
    GLfloat posicaoLuz[4] = {0.0, 0.0, 45.0, 1.0};
    GLfloat posicaoLuz1[4] = {0.0, 0.0, 35.0, 1.0};
    GLfloat posicaoLuz2[4] = {5.0, 0.0, 40.0, 1.0};
    GLfloat posicaoLuz3[4] = {-5.0, 0.0, 40.0, 1.0};
    GLfloat posicaoLuz4[4] = {0.0, 5.0, 40.0, 1.0};
    GLfloat posicaoLuz5[4] = {0.0, -5.0, 40.0, 1.0};
    // Capacidade de brilho do material
    GLfloat especularidade[4] = {1.0, 1.0, 1.0, 1.0};
    GLint especMaterial = 60;
    // Define a reflet�ncia do material
    glMaterialfv(GL_FRONT, GL_SPECULAR, especularidade);
    // Define a concentra��o do brilho
    glMateriali(GL_FRONT, GL_SHININESS, especMaterial);
    // Ativa o uso da luz ambiente
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);
    // Define os par�metros da luz de n�mero 0
    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);

    glLightfv(GL_LIGHT1, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, luzDifusa);
    glLightfv(GL_LIGHT1, GL_SPECULAR, luzEspecular);
    glLightfv(GL_LIGHT1, GL_POSITION, posicaoLuz1);

    glLightfv(GL_LIGHT2, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, luzDifusa);
    glLightfv(GL_LIGHT2, GL_SPECULAR, luzEspecular);
    glLightfv(GL_LIGHT2, GL_POSITION, posicaoLuz2);

    glLightfv(GL_LIGHT3, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, luzDifusa);
    glLightfv(GL_LIGHT3, GL_SPECULAR, luzEspecular);
    glLightfv(GL_LIGHT3, GL_POSITION, posicaoLuz3);

    glLightfv(GL_LIGHT4, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT4, GL_DIFFUSE, luzDifusa);
    glLightfv(GL_LIGHT4, GL_SPECULAR, luzEspecular);
    glLightfv(GL_LIGHT4, GL_POSITION, posicaoLuz4);

    glLightfv(GL_LIGHT5, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT5, GL_DIFFUSE, luzDifusa);
    glLightfv(GL_LIGHT5, GL_SPECULAR, luzEspecular);
    glLightfv(GL_LIGHT5, GL_POSITION, posicaoLuz5);
}

// Fun��o callback de redesenho da janela de visualiza��o
void Desenha(void)
{

    // Limpa a janela de visualiza��o com a cor
    // de fundo definida previamente
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glRotatef(lookx,0,0,0);

    DefineIluminacao();

    desenhaTerreno();

    // Altera a cor do desenho para branco
    glColor3f(1.0f, 1.0f, 1.0f);

    glPushMatrix();
    glTranslated(0, 0, 40);

    // Sol
    DesenhaEstatico(1, 1, 0, 3);

    glPushMatrix();
    glTranslated(deslocamentoX, deslocamentoY + cos(looky), deslocamentoZ + sin(looky)-40);
    DesenhaEstatico(1, 1, 0, 0.01);
    glPopMatrix();

    // Desenha os 8 planetas
    // e 8 orbitas
    // satelites e aneis
    for (int planeta = 1; planeta <= 8; planeta++)
    {
        glPushMatrix();

        if (showOrbita)
            DesenhaOrbita(10 * planeta);
        // glRotatef(angs[planeta-1],0.0f,0.0f,1.0f);
        glTranslated(20 * planeta * cos(angsPlaneta[planeta - 1]), 10 * planeta * sin(angsPlaneta[planeta - 1]), 0);
        if (show[planeta - 1])
            DesenhaPlaneta(planeta - 1, 10 * planeta);
        glPopMatrix();
    }

    glPopMatrix();

    // Executa os comandos OpenGL
    glutSwapBuffers();
}

// Fun��o usada para especificar o volume de visualiza��o
void EspecificaParametrosVisualizacao(void)
{
    // Especifica sistema de coordenadas de proje��o
    glMatrixMode(GL_PROJECTION);
    // Inicializa sistema de coordenadas de proje��o
    glLoadIdentity();

    // Especifica a proje��o perspectiva(angulo,aspecto,zMin,zMax)
    gluPerspective(angle, fAspect, 0.5, 500);

    // Especifica sistema de coordenadas do modelo
    glMatrixMode(GL_MODELVIEW);
    // Inicializa sistema de coordenadas do modelo
    glLoadIdentity();

    printf("Camera %2f, %2f, %2f\n", deslocamentoX, deslocamentoY + cos(looky), deslocamentoZ + sin(looky));

    int boop;
    int boop2;
    if(deslocamentoZ + sin(looky) >= deslocamentoZ){
        boop = 1;
    }
    if(deslocamentoY + cos(looky) >= deslocamentoY){
        boop2 = 0;
    }
    // Especifica posi��o do observador, do alvo e do vetor up
    gluLookAt(deslocamentoX, deslocamentoY, deslocamentoZ,
              deslocamentoX, deslocamentoY + cos(looky), deslocamentoZ + sin(looky),
              0, boop2, boop);
}

// Fun��o callback chamada quando o tamanho da janela � alterado
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
    // Para previnir uma divis�o por zero
    if (h == 0)
        h = 1;

    // Especifica as dimens�es da viewport
    glViewport(0, 0, w, h);

    // Calcula a corre��o de aspecto
    fAspect = (GLfloat)w / (GLfloat)h;

    EspecificaParametrosVisualizacao();
}

// Fun��o callback chamada para gerenciar eventos de teclas especiais(F1,PgDn,...)
void TeclasEspeciais(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP: // desloca o volume de visualiza��o para cima
        deslocamentoY += 2;
        break;
    case GLUT_KEY_DOWN: // desloca o volume de visualiza��o para baixo
        deslocamentoY -= 2;
        break;
    case GLUT_KEY_LEFT: // desloca o volume de visualiza��o para o lado esquerdo
        deslocamentoX -= 2;
        break;
    case GLUT_KEY_RIGHT: // desloca o volume de visualiza��o para o lado direito
        deslocamentoX += 2;
        break;
    case GLUT_KEY_PAGE_UP: // desloca o volume de visualiza��o para frente
        deslocamentoZ += 2;
        break;
    case GLUT_KEY_PAGE_DOWN: // desloca o volume de visualiza��o para tr�s
        deslocamentoZ -= 2;
        break;
    }
    EspecificaParametrosVisualizacao();
    glutPostRedisplay();
}

void Anima(int value)
{

    /*
     * Alterar os angulos respectivos dos satelites e planetas
     * A cada chamada do metodo `Anima'
     */
    double base = 0.00104;

    for (int planeta = 0; planeta < 8; planeta++)
    {
        angsSatelite[planeta] += base * velocidadesR[planeta];
    }

    for (int planeta = 0; planeta < 8; planeta++)
    {
        angsPlaneta[planeta] += base * velocidadesT[planeta];
    }

    /*
     * Calculo da Translacao do planeta Terra
     */
    tempoTerra += 10;

    if (angsPlaneta[2] > 6.28 && !showedTempoTerra)
    {
        showedTempoTerra = true;
        printf("A Terra levou %5d segundos para dar uma volta no sol", tempoTerra / 100);
    }

    glutPostRedisplay();
    if (animate)
        glutTimerFunc(10, Anima, 1);
}

// Fun��o callback chamada quando s�o notificados os eventos do mouse
void GerenciaMouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
        if (state == GLUT_DOWN)
        { // zoom in
            if (angle >= 10)
                angle -= 5;
        }
    if (button == GLUT_RIGHT_BUTTON)
        if (state == GLUT_DOWN)
        { // zoom out
            if (angle <= 130)
                angle += 5;
        }
    EspecificaParametrosVisualizacao();
    glutPostRedisplay();
}

// Fun��o callback chamada para gerenciar eventos de teclas
void Teclado(unsigned char key, int x, int y)
{
    /*
     * Controle de visualizacao dos Planetas
     * Para as teclas alfa-numericas de 1 a 8
     */
    show[key - 49] = !show[key - 49];

    /*
     * Controle de visualizacao das Orbitas
     * Para as teclas `o' ou `O'
     */
    if (key == 79 || key == 111)
    {
        showOrbita = !showOrbita;
    }

    /*
     * Controle de animacao do sistema solar
     * Para as teclas `p' ou `P'
     */
    if (key == 80 || key == 112)
    {
        animate = !animate;
        if (animate)
        {
            glutTimerFunc(100, Anima, 1);
        }
    }
    // a
    if (key == 97)
    {
        lookx -= 0.05;
    }
    // d
    if (key == 100)
    {
        lookx += 0.05;
    }
    // w
    if (key == 119)
    {
        looky += 0.05;
    }
    // s
    if (key == 115)
    {
        looky -= 0.05;
    }

    EspecificaParametrosVisualizacao();

    glutPostRedisplay();
}

// Fun��o respons�vel por inicializar par�metros e vari�veis
void Inicializa(void)
{
    // Define a cor de fundo da janela de visualização como preta
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    // Habilita a luz de n�mero 0
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_LIGHT3);
    glEnable(GL_LIGHT4);
    glEnable(GL_LIGHT5);
    glEnable(GL_DEPTH_TEST);

    // Inicializa a vari�vel que especifica o �ngulo da proje��o
    // perspectiva
    angle = 60;

    // Inicializa as vari�veis utilizadas para implementa��o
    // da opera��o de pan
    deslocamentoX = 0.0f;
    deslocamentoY = -100.0f;
    deslocamentoZ = 20.0f;
}

// Programa Principal
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    // Define do modo de opera��o da GLUT
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    // Especifica a posi��o inicial da janela GLUT
    glutInitWindowPosition(5, 5);

    // Especifica o tamanho inicial em pixels da janela GLUT
    glutInitWindowSize(500, 500);

    // Cria a janela passando como argumento o t�tulo da mesma
    glutCreateWindow("Sistema solar 3d");

    // Registra a fun��o callback de redesenho da janela de visualiza��o
    glutDisplayFunc(Desenha);

    // Registra a fun��o callback de redimensionamento da janela de visualiza��o
    glutReshapeFunc(AlteraTamanhoJanela);

    // Registra a fun��o callback para tratamento das teclas especiais
    glutSpecialFunc(TeclasEspeciais);

    // Registra a fun��o callback que gerencia os eventos do mouse
    glutMouseFunc(GerenciaMouse);

    // Registra a fun��o callback para tratamento das teclas ASCII
    glutKeyboardFunc(Teclado);

    glutTimerFunc(100, Anima, 1);

    // Chama a fun��o respons�vel por fazer as inicializa��es
    Inicializa();

    // Inicia o processamento e aguarda intera��es do usu�rio
    glutMainLoop();

    return 0;
}
