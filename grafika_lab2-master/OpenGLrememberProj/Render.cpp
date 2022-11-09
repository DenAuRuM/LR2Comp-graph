#include "Render.h"

#include <sstream>
#include <iostream>
#include <list>

#include <windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>

#include "MyOGL.h"

#include "Camera.h"
#include "Light.h"
#include "Primitives.h"

#include "GUItextRectangle.h"
using namespace std;


#define _height 4
#define pairDD pair<double, double>



void func();
void round(double step, double pointA[], double pointB[], double pointC[]);
void roundIn(double step, double pointA[], double pointB[], double pointC[], double pointA1[], double E[], double H[]);
void lineFromPoints(pairDD P, pairDD Q, double& a, double& b, double& c);
void perpendicularBisectorFromLine(pairDD P, pairDD Q, double& a, double& b, double& c);
pairDD lineLineIntersection(double a1, double b1, double c1, double a2, double b2, double c2);
pairDD findCircumCenter(pairDD P, pairDD Q, pairDD R);

list<double> vectProz(double A[], double B[], double C[], int k);

int switcher = 0;

bool textureMode = true;
bool lightMode = true;

double A1_texture[] = { 0.768, 0.306 };
double B1_texture[] = { 1.0, 0.615 };
double C1_texture[] = { 0.69, 0.922 };
double D1_texture[] = { 0.46, 0.615 };
double E1_texture[] = { 0.156, 0.766 };
double F1_texture[] = { 0.0, 0.384 };
double G1_texture[] = { 0.309, 0.078 };
double H1_texture[] = { 0.461, 0.461};
double pointC1_texture[] = { 0.769, 0.691 };


double A_texture[] = { 0.768, 0.306 };
double B_texture[] = { 1.0, 0.615 };
double C_texture[] = { 0.69, 0.922 };
double D_texture[] = { 0.46, 0.615 };
double E_texture[] = { 0.156, 0.766 };
double F_texture[] = { 0.0, 0.384 };
double G_texture[] = { 0.309, 0.078 };
double H_texture[] = { 0.461, 0.461 };
double pointC_texture[] = { 0.769, 0.691 };



double Ab_texture[] = { 0.0, 0.0 };
double Bb_texture[] = { 0.0, 0.0 };
double Cb_texture[] = { 0.0, 0.0 };
double Db_texture[] = { 0.0, 0.0 };
double Eb_texture[] = { 0.0, 0.0 };
double Fb_texture[] = { 0.0, 0.0 };


double Gb_texture[] = { 0.0, 0.0 };
double Hb_texture[] = { 0.0, 0.0 };

double A1b_texture[] = { 0.0, 0.0 };
double B1b_texture[] = { 0.0, 0.0 };
double C1b_texture[] = { 0.0, 0.0 };
double D1b_texture[] = { 0.0, 0.0 };
double E1b_texture[] = { 0.0, 0.0 };
double F1b_texture[] = { 0.0, 0.0 };

double G1b_texture[] = { 0.0, 0.0 };
double H1b_texture[] = { 0.0, 0.0 };


double F1add_texture[] = { 0.0, 0.0 };
double Fadd_texture[] = { 0.0, 0.0 };

double A1add_texture[] = { 0.0, 0.0 };
double Aadd_texture[] = { 0.0, 0.0 };


//класс для настройки камеры
class CustomCamera : public Camera
{
public:
	//дистанция камеры
	double camDist;
	//углы поворота камеры
	double fi1, fi2;


	//значния масеры по умолчанию
	CustomCamera()
	{
		camDist = 15;
		fi1 = 1;
		fi2 = 1;
	}


	//считает позицию камеры, исходя из углов поворота, вызывается движком
	void SetUpCamera()
	{
		//отвечает за поворот камеры мышкой
		lookPoint.setCoords(0, 0, 0);

		pos.setCoords(camDist * cos(fi2) * cos(fi1),
			camDist * cos(fi2) * sin(fi1),
			camDist * sin(fi2));

		if (cos(fi2) <= 0)
			normal.setCoords(0, 0, -1);
		else
			normal.setCoords(0, 0, 1);

		LookAt();
	}

	void CustomCamera::LookAt()
	{
		//функция настройки камеры
		gluLookAt(pos.X(), pos.Y(), pos.Z(), lookPoint.X(), lookPoint.Y(), lookPoint.Z(), normal.X(), normal.Y(), normal.Z());
	}



}  camera;   //создаем объект камеры


//Класс для настройки света
class CustomLight : public Light
{
public:
	CustomLight()
	{
		//начальная позиция света
		pos = Vector3(1, 1, 3);
	}


	//рисует сферу и линии под источником света, вызывается движком
	void  DrawLightGhismo()
	{
		glDisable(GL_LIGHTING);


		glColor3d(0.9, 0.8, 0);
		Sphere s;
		s.pos = pos;
		s.scale = s.scale * 0.08;
		s.Show();

		if (OpenGL::isKeyPressed('G'))
		{
			glColor3d(0, 0, 0);
			//линия от источника света до окружности
			glBegin(GL_LINES);
			glVertex3d(pos.X(), pos.Y(), pos.Z());
			glVertex3d(pos.X(), pos.Y(), 0);
			glEnd();

			//рисуем окруность
			Circle c;
			c.pos.setCoords(pos.X(), pos.Y(), 0);
			c.scale = c.scale * 1.5;
			c.Show();
		}

	}

	void SetUpLight()
	{
		GLfloat amb[] = { 0.2, 0.2, 0.2, 0 };
		GLfloat dif[] = { 1.0, 1.0, 1.0, 0 };
		GLfloat spec[] = { .7, .7, .7, 0 };
		GLfloat position[] = { pos.X(), pos.Y(), pos.Z(), 1. };

		// параметры источника света
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		// характеристики излучаемого света
		// фоновое освещение (рассеянный свет)
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
		// диффузная составляющая света
		glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
		// зеркально отражаемая составляющая света
		glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

		glEnable(GL_LIGHT0);
	}


} light;  //создаем источник света




//старые координаты мыши
int mouseX = 0, mouseY = 0;

void mouseEvent(OpenGL* ogl, int mX, int mY)
{
	int dx = mouseX - mX;
	int dy = mouseY - mY;
	mouseX = mX;
	mouseY = mY;

	//меняем углы камеры при нажатой левой кнопке мыши
	if (OpenGL::isKeyPressed(VK_RBUTTON))
	{
		camera.fi1 += 0.01 * dx;
		camera.fi2 += -0.01 * dy;
	}


	//двигаем свет по плоскости, в точку где мышь
	if (OpenGL::isKeyPressed('G') && !OpenGL::isKeyPressed(VK_LBUTTON))
	{
		LPPOINT POINT = new tagPOINT();
		GetCursorPos(POINT);
		ScreenToClient(ogl->getHwnd(), POINT);
		POINT->y = ogl->getHeight() - POINT->y;

		Ray r = camera.getLookRay(POINT->x, POINT->y);

		double z = light.pos.Z();

		double k = 0, x = 0, y = 0;
		if (r.direction.Z() == 0)
			k = 0;
		else
			k = (z - r.origin.Z()) / r.direction.Z();

		x = k * r.direction.X() + r.origin.X();
		y = k * r.direction.Y() + r.origin.Y();

		light.pos = Vector3(x, y, z);
	}

	if (OpenGL::isKeyPressed('G') && OpenGL::isKeyPressed(VK_LBUTTON))
	{
		light.pos = light.pos + Vector3(0, 0, 0.02 * dy);
	}


}

void mouseWheelEvent(OpenGL* ogl, int delta)
{

	if (delta < 0 && camera.camDist <= 1)
		return;
	if (delta > 0 && camera.camDist >= 100)
		return;

	camera.camDist += 0.01 * delta;

}

GLuint texId;

void keyDownEvent(OpenGL* ogl, int key)
{


	if (key == 'L')
	{
		lightMode = !lightMode;
	}

	if (key == 'T')
	{
		textureMode = !textureMode;
	}

	if (key == 'R')
	{
		camera.fi1 = 1;
		camera.fi2 = 1;
		camera.camDist = 15;

		light.pos = Vector3(1, 1, 3);
	}

	if (key == 'F')
	{
		light.pos = camera.pos;
	}

}

void keyUpEvent(OpenGL* ogl, int key)
{

}





//выполняется перед первым рендером
void initRender(OpenGL* ogl)
{
	//настройка текстур

	//4 байта на хранение пикселя
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	//настройка режима наложения текстур
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//включаем текстуры
	glEnable(GL_TEXTURE_2D);


	//массив трехбайтных элементов  (R G B)
	RGBTRIPLE* texarray;

	//массив символов, (высота*ширина*4      4, потомучто   выше, мы указали использовать по 4 байта на пиксель текстуры - R G B A)
	char* texCharArray;
	int texW, texH;
	OpenGL::LoadBMP("tx1.bmp", &texW, &texH, &texarray);
	OpenGL::RGBtoChar(texarray, texW, texH, &texCharArray);



	//генерируем ИД для текстуры
	glGenTextures(1, &texId);
	//биндим айдишник, все что будет происходить с текстурой, будте происходить по этому ИД
	glBindTexture(GL_TEXTURE_2D, texId);

	//загружаем текстуру в видеопямять, в оперативке нам больше  она не нужна
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCharArray);

	//отчистка памяти
	free(texCharArray);
	free(texarray);

	//наводим шмон
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	//камеру и свет привязываем к "движку"
	ogl->mainCamera = &camera;
	ogl->mainLight = &light;

	// нормализация нормалей : их длины будет равна 1
	glEnable(GL_NORMALIZE);

	// устранение ступенчатости для линий
	glEnable(GL_LINE_SMOOTH);


	//   задать параметры освещения
	//  параметр GL_LIGHT_MODEL_TWO_SIDE - 
	//                0 -  лицевые и изнаночные рисуются одинаково(по умолчанию), 
	//                1 - лицевые и изнаночные обрабатываются разными режимами       
	//                соответственно лицевым и изнаночным свойствам материалов.    
	//  параметр GL_LIGHT_MODEL_AMBIENT - задать фоновое освещение, 
	//                не зависящее от сточников
	// по умолчанию (0.2, 0.2, 0.2, 1.0)

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);

	camera.fi1 = -1.3;
	camera.fi2 = 0.8;
}


void Render(OpenGL* ogl)
{



	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glEnable(GL_DEPTH_TEST);
	if (textureMode)
		glEnable(GL_TEXTURE_2D);

	if (lightMode)
		glEnable(GL_LIGHTING);


	//альфаналожение
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//настройка материала
	GLfloat amb[] = { 0.2, 0.2, 0.1, 1. };
	GLfloat dif[] = { 0.4, 0.65, 0.5, 1. };
	GLfloat spec[] = { 0.9, 0.8, 0.3, 1. };
	GLfloat sh = 0.1f * 256;


	//фоновая
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	//дифузная
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	//зеркальная
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec); \
		//размер блика
		glMaterialf(GL_FRONT, GL_SHININESS, sh);

	//чтоб было красиво, без квадратиков (сглаживание освещения)
	glShadeModel(GL_SMOOTH);
	//===================================
	//Прогать тут  
	func();



	//Сообщение вверху экрана


	glMatrixMode(GL_PROJECTION);	//Делаем активной матрицу проекций. 
	//(всек матричные операции, будут ее видоизменять.)
	glPushMatrix();   //сохраняем текущую матрицу проецирования (которая описывает перспективную проекцию) в стек 				    
	glLoadIdentity();	  //Загружаем единичную матрицу
	glOrtho(0, ogl->getWidth(), 0, ogl->getHeight(), 0, 1);	 //врубаем режим ортогональной проекции

	glMatrixMode(GL_MODELVIEW);		//переключаемся на модел-вью матрицу
	glPushMatrix();			  //сохраняем текущую матрицу в стек (положение камеры, фактически)
	glLoadIdentity();		  //сбрасываем ее в дефолт

	glDisable(GL_LIGHTING);



	GuiTextRectangle rec;		   //классик моего авторства для удобной работы с рендером текста.
	rec.setSize(300, 150);
	rec.setPosition(10, ogl->getHeight() - 150 - 10);


	std::stringstream ss;
	ss << "T - вкл/выкл текстур" << std::endl;
	ss << "L - вкл/выкл освещение" << std::endl;
	ss << "F - Свет из камеры" << std::endl;
	ss << "G - двигать свет по горизонтали" << std::endl;
	ss << "G+ЛКМ двигать свет по вертекали" << std::endl;
	ss << "Коорд. света: (" << light.pos.X() << ", " << light.pos.Y() << ", " << light.pos.Z() << ")" << std::endl;
	ss << "Коорд. камеры: (" << camera.pos.X() << ", " << camera.pos.Y() << ", " << camera.pos.Z() << ")" << std::endl;
	ss << "Параметры камеры: R=" << camera.camDist << ", fi1=" << camera.fi1 << ", fi2=" << camera.fi2 << std::endl;

	rec.setText(ss.str().c_str());
	rec.Draw();

	glMatrixMode(GL_PROJECTION);	  //восстанавливаем матрицы проекции и модел-вью обратьно из стека.
	glPopMatrix();


	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

}

void func()
{
	double A[] = { 4, -3,  0 };
	double B[] = { 7, 1, 0 };
	double C[] = { 3, 5, 0 };
	double D[] = { 0, 1, 0 };
	double E[] = { -4, 3, 0 };
	double F[] = { -6, -2, 0 };
	double G[] = { -2, -6, 0 };
	double H[] = { 0, -1, 0 };


	double A1[] = { 4, -3,   _height };
	double B1[] = { 7, 1, _height };
	double C1[] = { 3, 5, _height };
	double D1[] = { 0, 1, _height };
	double E1[] = { -4, 3, _height };
	double F1[] = { -6, -2, _height };
	double G1[] = { -2, -6, _height };
	double H1[] = { 0, -1, _height };

	list<double> N;
	double a, b, c;
	auto iter = N.begin();

	glBindTexture(GL_TEXTURE_2D, texId);
	glBegin(GL_TRIANGLES);
	N = vectProz(G, F, H, 1);
	iter = N.begin();
	a = *iter;
	b = *++iter;
	c = *++iter;
	glNormal3d(a, b, c);
	glColor3d(0.2, 0.7, 0);
	glTexCoord2d(G_texture[0], G_texture[1]);
	glVertex3dv(G);
	glTexCoord2d(F_texture[0], F_texture[1]);
	glVertex3dv(F);
	glTexCoord2d(H_texture[0], H_texture[1]);
	glVertex3dv(H);

	N = vectProz(F, E, D, 1);

	iter = N.begin();
	a = *iter;
	b = *++iter;
	c = *++iter;
	glNormal3d(a, b, c);
	glColor3d(0.9, 0.9, 0.9);
	glTexCoord2d(F_texture[0], F_texture[1]);
	glVertex3dv(F);
	glTexCoord2d(E_texture[0], E_texture[1]);
	glVertex3dv(E);
	glTexCoord2d(D_texture[0], D_texture[1]);
	glVertex3dv(D);

	N = vectProz(F, D, H, 1);

	iter = N.begin();
	a = *iter;
	b = *++iter;
	c = *++iter;
	glNormal3d(a, b, c);
	glColor3d(0.4, 0.7, 0.9);
	glTexCoord2d(F_texture[0], F_texture[1]);
	glVertex3dv(F);
	glTexCoord2d(D_texture[0], D_texture[1]);
	glVertex3dv(D);
	glTexCoord2d(H_texture[0], H_texture[1]);
	glVertex3dv(H);

	N = vectProz(A, D, H, -1);

	iter = N.begin();
	a = *iter;
	b = *++iter;
	c = *++iter;
	glNormal3d(a, b, c);
	glColor3d(0.7, 0.5, 0.2);
	glTexCoord2d(A_texture[0], A_texture[1]);
	glVertex3dv(A);
	glTexCoord2d(D_texture[0], D_texture[1]);
	glVertex3dv(D);
	glTexCoord2d(H_texture[0], H_texture[1]);
	glVertex3dv(H);
	glEnd();

	round(0.001, F, G, A1);
	glTexCoord2d(pointC_texture[0], pointC_texture[1]);
	double pointC[] = { 4, 2 ,0 };
	roundIn(0.01, B, C, pointC, A1, A, D);

	//double step = abs(_height / (oboroty / fi));


	glBindTexture(GL_TEXTURE_2D, texId);
	glBegin(GL_QUADS);
	N = vectProz(E, E1, D1, 1);
	iter = N.begin();
	a = *iter;
	b = *++iter;
	c = *++iter;
	glNormal3d(a, b, c);
	glColor3d(0.1, 0.1, 0.9);
	glTexCoord2d(E_texture[0], E_texture[1]);
	glVertex3dv(E);
	glTexCoord2d(E1_texture[0], E1_texture[1]);
	glVertex3dv(E1);
	glTexCoord2d(D1_texture[0], D1_texture[1]);
	glVertex3dv(D1);
	glTexCoord2d(D_texture[0], D_texture[1]);
	glVertex3dv(D);

	N = vectProz(C, C1, D1, -1);

	iter = N.begin();
	a = *iter;
	b = *++iter;
	c = *++iter;
	glNormal3d(a, b, c);
	glColor3d(0.2, 0.2, 0.9);
	glVertex3dv(C);
	glVertex3dv(C1);
	glVertex3dv(D1);
	glVertex3dv(D);

	N = vectProz(B, B1, A1, 1);

	iter = N.begin();
	a = *iter;
	b = *++iter;
	c = *++iter;
	glNormal3d(a, b, c);
	glColor3d(0.4, 0.4, 0.9);
	glVertex3dv(B);
	glVertex3dv(B1);
	glVertex3dv(A1);
	glVertex3dv(A);

	N = vectProz(A, A1, H1, 1);

	iter = N.begin();
	a = *iter;
	b = *++iter;
	c = *++iter;
	glNormal3d(a, b, c);
	glColor3d(0.5, 0.5, 0.9);
	glVertex3dv(A);
	glVertex3dv(A1);
	glVertex3dv(H1);
	glVertex3dv(H);

	N = vectProz(G, G1, H1, -1);

	iter = N.begin();
	a = *iter;
	b = *++iter;
	c = *++iter;
	glNormal3d(a, b, c);
	glColor3d(0.6, 0.6, 0.9);
	glVertex3dv(G);
	glVertex3dv(G1);
	glVertex3dv(H1);
	glVertex3dv(H);

	N = vectProz(F, F1, E1, 1);

	iter = N.begin();
	a = *iter;
	b = *++iter;
	c = *++iter;
	glNormal3d(a, b, c);
	glColor3d(0.8, 0.8, 0.9);
	glVertex3dv(F);
	glVertex3dv(F1);
	glVertex3dv(E1);
	glVertex3dv(E);

	N = vectProz(E, E1, D1, 1);

	iter = N.begin();
	a = *iter;
	b = *++iter;
	c = *++iter;
	glNormal3d(a, b, c);
	glColor3d(0.9, 0.9, 0.9);
	glVertex3dv(E);
	glVertex3dv(E1);
	glVertex3dv(D1);
	glVertex3dv(D);
	glEnd();








	glBindTexture(GL_TEXTURE_2D, texId);
	glBegin(GL_TRIANGLES);

	N = vectProz(G1, F1, H1, -1);

	iter = N.begin();
	a = *iter;
	b = *++iter;
	c = *++iter;
	glNormal3d(a, b, c);
	glColor3d(0.2, 0.7, 0);
	glTexCoord2d(G1_texture[0], G1_texture[1]);
	glVertex3dv(G1);
	glTexCoord2d(F1_texture[0], F1_texture[1]);
	glVertex3dv(F1);
	glTexCoord2d(H1_texture[0], H1_texture[1]);
	glVertex3dv(H1);

	N = vectProz(F1, D1, H1, -1);

	iter = N.begin();
	a = *iter;
	b = *++iter;
	c = *++iter;
	glNormal3d(a, b, c);
	glColor3d(0.4, 0.7, 0.9);
	glTexCoord2d(F1_texture[0], F1_texture[1]);
	glVertex3dv(F1);
	glTexCoord2d(D1_texture[0], D1_texture[1]);
	glVertex3dv(D1);
	glTexCoord2d(H1_texture[0], H1_texture[1]);
	glVertex3dv(H1);


	N = vectProz(F1, E1, D1, -1);

	iter = N.begin();
	a = *iter;
	b = *++iter;
	c = *++iter;
	glNormal3d(a, b, c);
	glColor3d(0.9, 0.9, 0.9);
	glTexCoord2d(F1_texture[0], F1_texture[1]);
	glVertex3dv(F1);
	glTexCoord2d(E1_texture[0], E1_texture[1]);
	glVertex3dv(E1);
	glTexCoord2d(D1_texture[0], D1_texture[1]);
	glVertex3dv(D1);




	N = vectProz(A1, D1, H1, 1);

	iter = N.begin();
	a = *iter;
	b = *++iter;
	c = *++iter;
	glNormal3d(a, b, c);
	glColor3d(0.7, 0.5, 0.2);
	glTexCoord2d(A1_texture[0], A1_texture[1]);
	glVertex3dv(A1);
	glTexCoord2d(D1_texture[0], D1_texture[1]);
	glVertex3dv(D1);
	glTexCoord2d(H1_texture[0], H1_texture[1]);
	glVertex3dv(H1);
	glEnd();



}


void round(double step, double pointA[], double pointB[], double pointC[])
{
	glColor3d(0, 0, 0);

	double vect_AB[] = { pointA[0] - pointB[0], pointA[1] - pointB[1] };
	double centre[] = { (pointA[0] + pointB[0]) / 2,  (pointA[1] + pointB[1]) / 2 };

	double length = sqrt(vect_AB[0] * vect_AB[0] + vect_AB[1] * vect_AB[1]);

	double radius = length / 2;

	double Fi = acos(vect_AB[0] / (length + 1));



	double vect_ABT[] = { (-0.891 + 0.739), 0.225 - 0.245 };
	double centreT[] = { (0.891 + 0.739) / 2,  (0.225 + 0.245) / 2 };

	double lengthT = sqrt(vect_ABT[0] * vect_ABT[0] + vect_ABT[1] * vect_ABT[1]);

	double radiusT = lengthT / 2;




	double vect_ABT1[] = { (0.302 - 0.453), 0.225 - 0.245 };
	double centreT1[] = { (0.302 + 0.453) / 2,  (0.225 + 0.245) / 2 };

	double lengthT1 = sqrt(vect_ABT1[0] * vect_ABT1[0] + vect_ABT1[1] * vect_ABT1[1]);

	double radiusT1 = lengthT1 / 2;

	glNormal3d(0, 0, -1);
	glBegin(GL_TRIANGLE_FAN);
	for (double i = Fi + 0.137; i <= 3.283 + Fi; i += step)
	{

		double point[] = { radius * cos(i) + centre[0], radius * sin(i) + centre[1], pointA[2] };
		double pointT[] = { radiusT * cos(-i + Fi + 0.28) + centreT[0], radiusT * sin(-i + Fi + 0.28) + centreT[1] };
		glTexCoord2d(pointT[0], pointT[1]);
		glVertex3dv(point);

	}



	glEnd();

	glNormal3d(0, 0, 1);
	glBegin(GL_TRIANGLE_FAN);
	for (double i = Fi + 0.137; i <= 3.283 + Fi; i += step)
	{

		double point[] = { radius * cos(i) + centre[0], radius * sin(i) + centre[1], pointC[2] };
		double pointT1[] = { radiusT1 * cos(i + Fi + 0.28 + 3.14) + centreT1[0], radiusT1 * sin(i + Fi + 0.28 + 3.14) + centreT1[1] };
		glTexCoord2d(pointT1[0], pointT1[1]);
		glVertex3dv(point);

	}



	glEnd();

	list<double> N;
	double a, b, c;
	auto iter = N.begin();

	double counter = (0.299 - 0.023) / (3.14 / step);
	double stepper = counter;
	double oldCounter = 0;


	glColor3d(0.2, 0.2, 0.2);

	glBegin(GL_QUADS);

	for (double i = Fi + 0.137; i <= 3.283 + Fi; i += step)
	{
		double point[] = { radius * cos(i) + centre[0], radius * sin(i) + centre[1], pointA[2] };
		double point1[] = { radius * cos(i) + centre[0], radius * sin(i) + centre[1], pointC[2] };
		double point2[] = { radius * cos(i + step) + centre[0], radius * sin(i + step) + centre[1], pointA[2] };
		double point3[] = { radius * cos(i + step) + centre[0], radius * sin(i + step) + centre[1], pointC[2] };

		N = vectProz(point, point2, point3, 1);

		iter = N.begin();
		a = *iter;
		b = *++iter;
		c = *++iter;
		glNormal3d(a, b, c);
		if (0.299 >= 0.023 + counter)
		{
			glTexCoord2d(0.547, oldCounter + 0.023);
			glVertex3dv(point);
			glTexCoord2d(0.640, oldCounter + 0.023);
			glVertex3dv(point1);
			glTexCoord2d(0.640, counter + 0.023);
			glVertex3dv(point3);
			glTexCoord2d(0.547, counter + 0.023);
			glVertex3dv(point2);
			counter += stepper;
			oldCounter += stepper;
		}
		else
		{

			glVertex3dv(point);

			glVertex3dv(point1);

			glVertex3dv(point3);

			glVertex3dv(point2);
		}



	}
	glEnd();
}


void roundIn(double step, double pointA[], double pointB[], double pointC[], double pointA1[], double E[], double H[])
{
	pairDD P = make_pair(pointA[0], pointA[1]);
	pairDD Q = make_pair(pointB[0], pointB[1]);
	pairDD R = make_pair(pointC[0], pointC[1]);
	pairDD pointO = findCircumCenter(P, Q, R);

	pairDD P1_text = make_pair(0.258, 0.490);
	pairDD Q1_text = make_pair(0.407, 0.582);
	pairDD R1_text = make_pair(0.365, 0.513);
	pairDD pointO1_text = findCircumCenter(P1_text, Q1_text, R1_text);


	double c = sqrt(pow((pointB[0] - pointA[0]), 2) + pow((pointB[1] - pointA[1]), 2));//длины
	double a = sqrt(pow((pointC[0] - pointB[0]), 2) + pow((pointC[1] - pointB[1]), 2));//сторон
	double b = sqrt(pow((pointA[0] - pointC[0]), 2) + pow((pointA[1] - pointC[1]), 2));//треугольника
	double s = 0.5 * abs(pointA[0] * (pointB[1] - pointC[1]) - pointA[1] * (pointB[0] - pointC[0]) + pointB[0] * pointC[1] - pointB[1] * pointC[0]);//площадь треуг

	double radius = (a * b * c) / (4 * s);//радиус описанной окр

	double Fi1 = -0.3 - acos((-pointA[0]) / (sqrt(pow(pointA[0], 2) + pow(pointA[1], 2))));

	double Fi2 = 0.7 - acos((-pointB[0]) / (sqrt(pow(pointB[0], 2) + pow(pointB[1], 2))));


	double c1_tex = sqrt(pow((0.407 - 0.258), 2) + pow((0.582 - 0.490), 2));//длины
	double a1_tex = sqrt(pow((0.365 - 0.407), 2) + pow((0.513 - 0.582), 2));//сторон
	double b1_tex = sqrt(pow((0.258 - 0.365), 2) + pow((0.490 - 0.513), 2));//треугольника
	double s1_tex = 0.5 * abs(0.258 * (0.582 - 0.513) - 0.490 * (0.407 - 0.365) + 0.407 * 0.513 - 0.582 * 0.365);//площадь треуг

	double radius1_tex = (a1_tex * b1_tex * c1_tex) / (4 * s1_tex);//радиус описанной окр


	glBindTexture(GL_TEXTURE_2D, texId);

	glBegin(GL_TRIANGLE_FAN);

	glNormal3d(0, 0, -1);

	glTexCoord2d(E_texture[0], E_texture[1]);
	glVertex3dv(E);
	for (double i = Fi1; i < Fi2; i += step)
		//for (double i = Fi1 - step - 0.515; i <= Fi2 - step + 0.35; i += step)
	{
		double point[] = { radius * cos(i) + pointO.first, radius * sin(i) + pointO.second, pointA[2] };

		glVertex3dv(point);




	}

	glVertex3dv(H);
	glEnd();


	list <double> N;
	auto iter = N.begin();
	glBindTexture(GL_TEXTURE_2D, texId);
	glBegin(GL_TRIANGLE_FAN);

	glNormal3d(0, 0, -1);

	glTexCoord2d(H_texture[0], H_texture[1]);
	glVertex3dv(H);
	for (double i = Fi1; i < Fi2; i += step)
		//for (double i = Fi1 - step - 0.515; i <= Fi2 - step + 0.35; i += step)
	{
		double point[] = { radius * cos(i) + pointO.first, radius * sin(i) + pointO.second, pointA[2] };


		glVertex3dv(point);




	}

	glEnd();

	glBindTexture(GL_TEXTURE_2D, texId);
	glBegin(GL_TRIANGLE_FAN);
	glNormal3d(0, 0, 1);
	double E1[3];
	E1[0] = E[0];
	E1[1] = E[1];
	E1[2] = pointA1[2];

	glTexCoord2d(0.407, 0.407);
	glTexCoord2d(E1_texture[0], E1_texture[1]);
	glVertex3dv(E1);
	for (double i = Fi1; i < Fi2; i += step)
		//for (double i = Fi1 - step - 0.515; i <= Fi2 - step + 0.35; i += step)
	{
		double point[] = { radius * cos(i) + pointO.first, radius * sin(i) + pointO.second, pointA1[2] };
		//double point_text[] = { radius1_tex * cos(i + 2.578 + 0.28) + pointO1_text.first, radius1_tex * sin(i + 2.578 + 0.28) + pointO1_text.second };
		double point_text[] = { radius1_tex * cos(-i + 4) + pointO1_text.first, radius1_tex * sin(-i + 4) + pointO1_text.second };
		glTexCoord2d(point_text[0], point_text[1]);
		glVertex3dv(point);




	}

	double H1[3];
	H1[0] = H[0];
	H1[1] = H[1];
	H1[2] = pointA1[2];
	glTexCoord2d(0.365, 0.385);
	glVertex3dv(H1);
	glEnd();



	glBegin(GL_TRIANGLE_FAN);

	glNormal3d(0, 0, 1);
	E1[0] = E[0];
	E1[1] = E[1];
	E1[2] = pointA1[2];
	glTexCoord2d(0.365, 0.385);
	glTexCoord2d(H1_texture[0], H1_texture[1]);
	glVertex3dv(H1);
	for (double i = Fi1; i < Fi2; i += step)
		//for (double i = Fi1 - step - 0.515; i <= Fi2 - step + 0.35; i += step)
	{
		double point[] = { radius * cos(i) + pointO.first, radius * sin(i) + pointO.second, pointA1[2] };
		//	double point_text[] = { radius1_tex * cos(-i - 3.14) + pointO1_text.first, radius1_tex * sin(-i - 3.14) + pointO1_text.second };
		double point_text[] = { radius1_tex * cos(-i - 9) + pointO1_text.first, radius1_tex * sin(-i - 9) + pointO1_text.second };
		glTexCoord2d(point_text[0], point_text[1]);
		glVertex3dv(point);




	}


	glEnd();



	double counter = 2 * (-0.5 + 0.717) / (3.14 / step); //Bb_texture[0] >= Ab_texture[0]
	double stepper = counter;
	double oldCounter = 0;

	glColor3d(1.0, 0.2, 0.2);

	glBegin(GL_QUADS);
	for (double i = Fi1; i < Fi2; i += step)
		//for (double i = Fi1 - step - 0.515; i <= Fi2 - step + 0.35; i += step)
	{
		double point[] = { radius * cos(i) + pointO.first, radius * sin(i) + pointO.second, pointA[2] };
		double point1[] = { radius * cos(i) + pointO.first, radius * sin(i) + pointO.second, pointA1[2] };
		double point2[] = { radius * cos(i + step) + pointO.first, radius * sin(i + step) + pointO.second, pointA[2] };
		double point3[] = { radius * cos(i + step) + pointO.first, radius * sin(i + step) + pointO.second, pointA1[2] };

		N = vectProz(point, point2, point3, -1);

		iter = N.begin();
		a = *iter;
		b = *++iter;
		c = *++iter;
		glNormal3d(a, b, c);
		if (0.299 >= 0.023 + counter)
		{
			glTexCoord2d(0.120, oldCounter + 0.5);
			glVertex3dv(point);
			glTexCoord2d(0.210, oldCounter + 0.5);
			glVertex3dv(point1);
			glTexCoord2d(0.210, counter + 0.5);
			glVertex3dv(point3);
			glTexCoord2d(0.120, counter + 0.5);
			glVertex3dv(point2);
			counter += stepper;
			oldCounter += stepper;
		}
		else
		{

			glVertex3dv(point);

			glVertex3dv(point1);

			glVertex3dv(point3);

			glVertex3dv(point2);
		}



	}
	glEnd();
}


// Function to find the line given two points
void lineFromPoints(pairDD P, pairDD Q, double& a, double& b, double& c)
{
	a = Q.second - P.second;
	b = P.first - Q.first;
	c = a * (P.first) + b * (P.second);
}

// Function which converts the input line to its
// perpendicular bisector. It also inputs the points
// whose mid-point lies on the bisector
void perpendicularBisectorFromLine(pairDD P, pairDD Q, double& a, double& b, double& c)
{
	pairDD mid_point = make_pair((P.first + Q.first) / 2,
		(P.second + Q.second) / 2);

	// c = -bx + ay
	c = -b * (mid_point.first) + a * (mid_point.second);

	double temp = a;
	a = -b;
	b = temp;
}

// Returns the intersection point of two lines
pairDD lineLineIntersection(double a1, double b1, double c1, double a2, double b2, double c2)
{
	double determinant = a1 * b2 - a2 * b1;
	if (determinant == 0)
	{
		// The lines are parallel. This is simplified
		// by returning a pair of FLT_MAX
		return make_pair(FLT_MAX, FLT_MAX);
	}

	else
	{
		double x = (b2 * c1 - b1 * c2) / determinant;
		double y = (a1 * c2 - a2 * c1) / determinant;
		return make_pair(x, y);
	}
}

pairDD findCircumCenter(pairDD P, pairDD Q, pairDD R)
{
	// Line PQ is represented as ax + by = c
	double a, b, c;
	lineFromPoints(P, Q, a, b, c);

	// Line QR is represented as ex + fy = g
	double e, f, g;
	lineFromPoints(Q, R, e, f, g);

	// Converting lines PQ and QR to perpendicular
	// vbisectors. After this, L = ax + by = c
	// M = ex + fy = g
	perpendicularBisectorFromLine(P, Q, a, b, c);
	perpendicularBisectorFromLine(Q, R, e, f, g);

	// The point of intersection of L and M gives
	// the circumcenter
	pairDD circumcenter =
		lineLineIntersection(a, b, c, e, f, g);

	if (circumcenter.first == FLT_MAX &&
		circumcenter.second == FLT_MAX)
	{
		cout << "The two perpendicular bisectors "
			"found come parallel" << endl;
		cout << "Thus, the given points do not form "
			"a triangle and are collinear" << endl;
	}

	else
	{
		cout << "The circumcenter of the triangle PQR is: ";
		cout << "(" << circumcenter.first << ", "
			<< circumcenter.second << ")" << endl;
	}


	return circumcenter;
}



list<double> vectProz(double A[], double B[], double C[], int k)
{
	double A1[] = { B[0] - A[0], B[1] - A[1], B[2] - A[2] };
	double B1[] = { C[0] - A[0], C[1] - A[1], C[2] - A[2] };

	list <double> list_ = { k * (A1[1] * B1[2] - A1[2] * B1[1]), k * (A1[2] * B1[0] - A1[0] * B1[2]), k * (A1[0] * B1[1] - A1[1] * B1[0]) };
	return list_;
}

