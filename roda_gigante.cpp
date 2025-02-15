/*
 Trabalho de Computação Gráfica - Simulação 3D de um brinquedo de parque
 Autor(es): Lucas Nojiri
 Descrição: Implementação de uma roda gigante 3D com iluminação, câmera e animação.
*/

#include <GL/glut.h>
#include <math.h>

// Variáveis globais para controle de animação e iluminação
bool isAnimating = false;        // Controla se a animação está ativa ou não
bool directionalLight = true;    // Controla se a luz direcional está ativa
bool pointLight = true;          // Controla se a luz pontual está ativa
float angle = 0.0f;              // Ângulo de rotação da roda gigante

// Variáveis para controle da câmera
float camRadius = 5.0f;  // Distância da câmera até o centro da roda gigante
float camAngle = 0.0f;   // Ângulo de rotação da câmera ao redor da roda gigante
float camY = 3.0f;       // Altura da câmera em relação ao chão

/*
 Função: updateCameraPosition
 Descrição: Atualiza a posição da câmera com base no raio, ângulo e altura definidos.
            A câmera orbita ao redor da roda gigante, mantendo o foco no centro.
*/
void updateCameraPosition() {
    float camX = camRadius * cos(camAngle);  // Calcula a posição X da câmera
    float camZ = camRadius * sin(camAngle);  // Calcula a posição Z da câmera
    gluLookAt(camX, camY, camZ, 0.0f, 0.0f, 0.0f, 0.0, 1.0, 0.0);  // Define a posição e o foco da câmera
}

/*
 Função: handleSpecialKeys
 Descrição: Manipula as teclas especiais (setas) para controlar a câmera.
            As setas esquerda e direita rotacionam a câmera ao redor da roda gigante.
            As setas para cima e para baixo ajustam a altura da câmera.
*/
void handleSpecialKeys(int key, int x, int y) {
    float rotationSpeed = 0.05f;  // Velocidade de rotação da câmera

    switch (key) {
        case GLUT_KEY_LEFT:  camAngle -= rotationSpeed; break;  // Rotaciona a câmera para a esquerda
        case GLUT_KEY_RIGHT: camAngle += rotationSpeed; break;  // Rotaciona a câmera para a direita
        case GLUT_KEY_UP:    camY += 0.2f; break;              // Aumenta a altura da câmera
        case GLUT_KEY_DOWN:  camY -= 0.2f; break;              // Diminui a altura da câmera
    }
    glutPostRedisplay();  // Solicita a atualização da tela
}

/*
 Função: drawCylinder
 Descrição: Desenha um cilindro 3D com base no raio da base, raio do topo e altura fornecidos.
            Utiliza a função gluCylinder da biblioteca GLU para criar o cilindro.
*/
void drawCylinder(float baseRadius, float topRadius, float height) {
    GLUquadric* quad = gluNewQuadric();  // Cria um novo objeto quadric
    gluCylinder(quad, baseRadius, topRadius, height, 20, 20);  // Desenha o cilindro
    gluDeleteQuadric(quad);  // Libera a memória do objeto quadric
}

/*
 Função: drawThickCircle3D
 Descrição: Desenha um círculo 3D espesso, composto por dois cilindros concêntricos.
            O cilindro externo representa o raio externo, e o interno representa o raio interno.
            A profundidade define a espessura do círculo.
*/
void drawThickCircle3D(float outerRadius, float innerRadius, float depth) {
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -depth / 2);  // Posiciona o cilindro externo
    drawCylinder(outerRadius, outerRadius, depth);  // Desenha o cilindro externo
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -depth / 2);  // Posiciona o cilindro interno
    drawCylinder(innerRadius, innerRadius, depth);  // Desenha o cilindro interno
    glPopMatrix();
}

/*
 Função: drawFerrisWheel3D
 Descrição: Desenha a roda gigante 3D, composta por um círculo espesso (aro) e cabines.
            As cabines são posicionadas uniformemente ao redor do aro e giram junto com ele.
*/
void drawFerrisWheel3D() {
    glColor3f(1.0f, 0.0f, 0.0f);  // Define a cor do aro da roda gigante
    drawThickCircle3D(0.9f, 0.75f, 0.1f);  // Desenha o aro da roda gigante

    glColor3f(0.0f, 0.0f, 1.0f);  // Define a cor das hastes de suporte
    for (int i = 0; i < 8; i++) {
        float angleR = 2.0f * M_PI * i / 8;  // Calcula o ângulo para cada haste

        glPushMatrix();
        glRotatef(angleR * 180.0f / M_PI, 0.0f, 0.0f, 1.0f);  // Rotaciona a haste
        glTranslatef(0.0f, 0.8f, 0.0f);  // Posiciona a haste
        glRotatef(90, 1.0f, 0.0f, 0.0f);  // Ajusta a orientação da haste
        drawCylinder(0.03f, 0.03f, 1.6f);  // Desenha a haste
        glPopMatrix();
    }

    glColor3f(1.0f, 1.0f, 0.0f);  // Define a cor das cabines
    for (int i = 0; i < 8; i++) {
        float angleR = 2.0f * M_PI * i / 8;  // Calcula o ângulo para cada cabine
        float x = 0.8f * cos(angleR);  // Calcula a posição X da cabine
        float y = 0.8f * sin(angleR);  // Calcula a posição Y da cabine
        
        glPushMatrix();
        glTranslatef(x, y, 0.0f);  // Posiciona a cabine
        glRotatef(-angle, 0.0f, 0.0f, 1.0f);  // Mantém a cabine nivelada durante a rotação
        glutSolidCube(0.1f);  // Desenha a cabine como um cubo sólido
        glPopMatrix();
    }
}

/*
 Função: drawGround
 Descrição: Desenha o chão da cena como um quadrado plano.
            O chão é posicionado abaixo da roda gigante e serve como referência visual.
*/
void drawGround() {
    glColor3f(0.0f, 0.5f, 0.0f);  // Define a cor do chão
    glBegin(GL_QUADS);
        glVertex3f(-5.0f, -1.2f, -5.0f);  // Vértice inferior esquerdo
        glVertex3f( 5.0f, -1.2f, -5.0f);  // Vértice inferior direito
        glVertex3f( 5.0f, -1.2f,  5.0f);  // Vértice superior direito
        glVertex3f(-5.0f, -1.2f,  5.0f);  // Vértice superior esquerdo
    glEnd();
}

/*
 Função: setupLights
 Descrição: Configura as fontes de luz da cena, incluindo luz direcional e luz pontual.
            A luz direcional simula a iluminação do sol, enquanto a luz pontual simula uma lâmpada.
*/
void setupLights() {
    glEnable(GL_LIGHTING);  // Habilita o uso de iluminação
    glShadeModel(GL_SMOOTH);  // Ativa Gouraud Shading para suavização da iluminação
    
    GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };  // Define a luz ambiente
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

    if (directionalLight) {
        glEnable(GL_LIGHT0);  // Habilita a luz direcional (LIGHT0)
        GLfloat lightPosition[] = { 1.0f, 1.0f, 1.0f, 0.0f };  // Define a posição da luz direcional
        GLfloat lightDiffuse[]  = { 1.0f, 0.9f, 0.7f, 1.0f };  // Tom mais quente
        GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };  // Reflexo branco

        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    } else {
        glDisable(GL_LIGHT0);  // Desabilita a luz direcional
    }

    if (pointLight) {
        glEnable(GL_LIGHT1);  // Habilita a luz pontual (LIGHT1)
        GLfloat pointPosition[] = { 0.0f, -2.0f, 0.0f, 1.0f };
        GLfloat pointDiffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
        GLfloat pointSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

        glLightfv(GL_LIGHT1, GL_POSITION, pointPosition);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, pointDiffuse);
        glLightfv(GL_LIGHT1, GL_SPECULAR, pointSpecular);
    } else {
        glDisable(GL_LIGHT1);  // Desabilita a luz pontual
    }
}


/*
 Função: update
 Descrição: Atualiza a animação da roda gigante, incrementando o ângulo de rotação.
            A função é chamada repetidamente para criar a animação contínua.
*/
void update(int value) {
    if (isAnimating) {
        angle += 2.0f;  // Incrementa o ângulo de rotação
        if (angle > 360.0f) angle -= 360.0f;  // Mantém o ângulo dentro do intervalo [0, 360]
    }
    glutPostRedisplay();  // Solicita a atualização da tela
    glutTimerFunc(16, update, 0);  // Agenda a próxima chamada da função após 16ms
}

/*
 Função: handleKeyboard
 Descrição: Manipula as teclas do teclado para controlar a animação e as luzes.
            A tecla 'a' inicia/pausa a animação, 's' alterna a luz direcional e 'l' alterna a luz pontual.
*/
void handleKeyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'a': isAnimating = !isAnimating; break;  // Alterna o estado da animação
        case 's': directionalLight = !directionalLight; break;  // Alterna a luz direcional
        case 'l': pointLight = !pointLight; break;  // Alterna a luz pontual
    }
    glutPostRedisplay();  // Solicita a atualização da tela
}

/*
 Função: init
 Descrição: Inicializa as configurações básicas do OpenGL, como o teste de profundidade e a cor de fundo.
*/
void init() {
    glShadeModel(GL_SMOOTH);  // Ativa Gouraud Shading para suavização da iluminação
    glEnable(GL_DEPTH_TEST);  // Habilita o teste de profundidade
    glEnable(GL_COLOR_MATERIAL);  // Habilita o uso de cores para materiais
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);  // Define a cor de fundo da cena
}

/*
 Função: setupProjection
 Descrição: Configura a projeção da cena, definindo a perspectiva e a matriz de projeção.
*/
void setupProjection() {
    glMatrixMode(GL_PROJECTION);  // Seleciona a matriz de projeção
    glLoadIdentity();  // Carrega a matriz identidade
    gluPerspective(45.0, 1.0, 1.0, 20.0);  // Define a projeção perspectiva
    glMatrixMode(GL_MODELVIEW);  // Retorna para a matriz de modelo
}

/*
 Função: display
 Descrição: Função principal de renderização da cena.
            Limpa o buffer, atualiza a câmera, configura as luzes e desenha os objetos.
*/
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Limpa os buffers de cor e profundidade
    glLoadIdentity();  // Carrega a matriz identidade
    
    updateCameraPosition();  // Atualiza a posição da câmera

    setupLights();  // Configura as luzes

    // Definição de material para melhor iluminação
    GLfloat materialDiffuse[]  = { 0.5f, 0.5f, 0.8f, 1.0f }; // Tom azulado
    GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // Reflexo branco forte
    GLfloat materialShininess  = 50.0f;  // Quanto maior, mais brilhante

    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

    drawGround();  // Desenha o chão

    // Base retangular da roda gigante
    glPushMatrix();
    glColor3f(0.6f, 0.3f, 0.1f); // Cor marrom para a base
    glTranslatef(0.0f, -1.2f, 0.0f); // Posição no chão
    glScalef(2.0f, 0.2f, 1.0f); // Escala para criar um retângulo alongado
    glutSolidCube(1.0f); // Desenha um cubo alongado
    glPopMatrix();

    // Adiciona dois tubos em pé acima do retângulo
    glPushMatrix();
    glColor3f(0.8f, 0.0f, 0.0f); // Cor vermelha para os tubos
    glTranslatef(-0.5f, -0.7f, 0.0f); // Posiciona o primeiro tubo
    glRotatef(90, 1.0f, 0.0f, 0.0f); // Rotaciona o tubo para ficar em pé
    drawCylinder(0.03f, 0.03f, 0.5f);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.8f, 0.0f, 0.0f); // Cor vermelha para os tubos
    glTranslatef(0.5f, -0.7f, 0.0f); // Posiciona o segundo tubo
    glRotatef(90, 1.0f, 0.0f, 0.0f); // Rotaciona o tubo para ficar em pé
    drawCylinder(0.03f, 0.03f, 0.5f);
    glPopMatrix();

    glPushMatrix();
    glRotatef(angle, 0.0f, 0.0f, 1.0f);  // Aplica a rotação da roda gigante
    drawFerrisWheel3D();  // Desenha a roda gigante
    glPopMatrix();

    glutSwapBuffers();  // Troca os buffers para exibir a cena renderizada
}

/*
 Função: main
 Descrição: Função principal do programa, responsável por inicializar a janela GLUT,
            configurar as funções de callback e iniciar o loop principal.
*/
int main(int argc, char** argv) {
    glutInit(&argc, argv);  // Inicializa a GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);  // Define o modo de exibição
    glutInitWindowSize(800, 800);  // Define o tamanho da janela
    glutCreateWindow("Roda Gigante OpenGL 3D - Movimentação e Iluminação");  // Cria a janela
    init();  // Inicializa as configurações do OpenGL
    setupProjection();  // Configura a projeção
    glutDisplayFunc(display);  // Define a função de renderização
    glutKeyboardFunc(handleKeyboard);  // Define a função de manipulação do teclado
    glutSpecialFunc(handleSpecialKeys);  // Define a função de manipulação das teclas especiais
    glutTimerFunc(16, update, 0);  // Configura o temporizador para a animação
    glutMainLoop();  // Inicia o loop principal da GLUT
    return 0;
}
