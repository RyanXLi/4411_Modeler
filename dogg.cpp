#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include <FL/gl.h>

#include "modelerglobals.h"

// To make a DoggModel, we inherit off of ModelerView
class DoggModel : public ModelerView 
{
public:
    DoggModel(int x, int y, int w, int h, char *label) 
        : ModelerView(x,y,w,h,label) { }

    virtual void draw();
};

// We need to make a creator function, mostly because of
// nasty API stuff that we'd rather stay away from.
ModelerView* createDoggModel(int x, int y, int w, int h, char *label)
{ 
    return new DoggModel(x,y,w,h,label); 
}

// We are going to override (is that the right word?) the draw()
// method of ModelerView to draw out DoggModel
void DoggModel::draw()
{
    // This call takes care of a lot of the nasty projection 
    // matrix stuff.  Unless you want to fudge directly with the 
	// projection matrix, don't bother with this ...
    ModelerView::draw();


    // draw axis
    glPushMatrix();
    glBegin(GL_LINES);
    glColor3f(1, 0, 0);
    glVertex3f(-50, -2, -20);
    glVertex3f(50, -2, -20);

    glColor3f(0, 1, 0);
    glVertex3f(0, -50, -20);
    glVertex3f(0, 50, -20);

    glColor3f(0, 0, 1);
    glVertex3f(0, -2, -100);
    glVertex3f(0, -2, 10);
    glEnd();
    glPopMatrix();


	// draw the dogg model
	setAmbientColor(.1f,.1f,.1f);
	setDiffuseColor(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslated(VAL(XPOS), VAL(YPOS), VAL(ZPOS));

        // part 1
		glPushMatrix();
		glTranslated(-2, 0, -1);
        glScaled(2, 3, 2);
		drawBox(1,1,1);
		glPopMatrix();


        // part 2
        glPushMatrix();
        glTranslated(0, 1.5, -1);
        glRotated(-90, 0.0, 0.0, 1.0);
        glScaled(1.5, 4.5, 2);
        drawTriangularPrism(1,1,1);
        glPopMatrix();

        glPushMatrix();
        glTranslated(0, 1.5, -1);
        glScaled(4.5, 1.5, 2);
        drawBox(1, 1, 1);
        glPopMatrix();

		//// draw cannon
		//glPushMatrix();
		//glRotated(VAL(ROTATE), 0.0, 1.0, 0.0);
		//glRotated(-90, 1.0, 0.0, 0.0);
		//drawCylinder(VAL(HEIGHT), 0.1, 0.1);
        //
		//glTranslated(0.0, 0.0, VAL(HEIGHT));
		//drawCylinder(1, 1.0, 0.9);
        //
		//glTranslated(0.0, 0.0, 0.5);
		//glRotated(90, 1.0, 0.0, 0.0);
		//drawCylinder(4, 0.1, 0.2);
		//glPopMatrix();

	glPopMatrix();
}



int main()
{
	// Initialize the controls
	// Constructor is ModelerControl(name, minimumvalue, maximumvalue, 
	// stepsize, defaultvalue)
    ModelerControl controls[NUMCONTROLS];
    controls[XPOS] = ModelerControl("X Position", -5, 5, 0.1f, 0);
    controls[YPOS] = ModelerControl("Y Position", 0, 5, 0.1f, 0);
    controls[ZPOS] = ModelerControl("Z Position", -5, 5, 0.1f, 0);
    controls[HEIGHT] = ModelerControl("Height", 1, 2.5, 0.1f, 1);
	controls[ROTATE] = ModelerControl("Rotate", -135, 135, 1, 0);

    ModelerApplication::Instance()->Init(&createDoggModel, controls, NUMCONTROLS);
    return ModelerApplication::Instance()->Run();
}
