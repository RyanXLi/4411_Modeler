#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include <FL/gl.h>
#include <math.h>

#include "modelerglobals.h"

// To make a DoggModel, we inherit off of ModelerView
class DoggModel : public ModelerView 
{
public:
    DoggModel(int x, int y, int w, int h, char *label) 
        : ModelerView(x,y,w,h,label) { }

    virtual void draw();
    void drawAxis();
    void resetLeg();
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


    //DoggModel::drawAxis();

    if (VAL(RESET_LEG)) { DoggModel::resetLeg(); }



	// draw the dogg model
	setAmbientColor(.1f,.1f,.1f);
	setDiffuseColor(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslated(VAL(XPOS), VAL(YPOS), VAL(ZPOS));

        // part 1: torso
		glPushMatrix();
		    glTranslated(-2, 0, -1);
            glScaled(2, 3, 2);
		    drawBox(1,1,1);
		glPopMatrix();


        // part 2: torso
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

        if (VAL(LV_DETAIL) > 1) {

            if (VAL(DRAW_PROP)) {
                // propeller
                // left
                glPushMatrix();
                glTranslated(1, 2.5, 0.9);
                glRotated(60, 1, 0, 0);

                glPushMatrix();
                glScaled(0.3, 1.2, 0.3);
                drawBox(1, 1, 1);
                glPopMatrix();

                glTranslated(-1.5, 0.8 - 0.2 + 0.8, 0);
                glPushMatrix();
                glRotated(90, 0, 1, 0);
                drawCylinder(3, 0.8, 0.8);
                glPopMatrix();
                glPopMatrix();

                // right
                glPushMatrix();
                glTranslated(1, 2.5 - 0.15, -0.9); // - (0.3 / 2) since rotation direction
                glRotated(-60, 1, 0, 0);

                glPushMatrix();
                glScaled(0.3, 1.2, 0.3);
                drawBox(1, 1, 1);
                glPopMatrix();

                glTranslated(-1.5, 0.8 - 0.2 + 0.8, +0.15);
                glPushMatrix();
                glRotated(90, 0, 1, 0);
                drawCylinder(3, 0.8, 0.8);
                glPopMatrix();
                glPopMatrix();

            }


            // part 3: neck

            double angle0 = 45 + VAL(NECK_ANGLE);

            glPushMatrix();

            glTranslated(-1, 1.3, -1);
            glRotated(angle0, 0.0, 0.0, 1.0);

            glPushMatrix();
            glScaled(1.5, 2.2, 2);
            drawBox(1, 1, 1);
            glPopMatrix();

            // head
            if (VAL(LV_DETAIL) > 2) {

                glTranslated(1.5, 2, 0);
                glRotated(90, 0, 0, 1);

                // rotate
                glTranslated(0, 0, 1);
                glRotated(VAL(HEAD_ANGLE_X), 1, 0, 0);
                glTranslated(0, 0, -1);
                glRotated(VAL(HEAD_ANGLE_Z), 0, 0, 1);

                glPushMatrix();
                glScaled(0.75, 3, 2);
                drawBox(1, 1, 1);
                glPopMatrix();

                glTranslated(0.75, 0, 0);

                glPushMatrix();
                glScaled(0.75, 2, 2);
                drawBox(1, 1, 1);
                glPopMatrix();
            }

            glPopMatrix();


            // part4, 8, 11 front right leg
            double frontLegSize = 0.6;
            double frontLegX = -2 + frontLegSize;
            double frontLegY = 0;
            double frontLegZ = -1;

            glPushMatrix();
                glTranslated(frontLegX, frontLegY + 0.5, frontLegZ);

                // rotate
                glRotated(VAL(RIGHT_FRONT_ANGLE1), 0, 0, 1);

                glPushMatrix();
                    glRotated(-180, 0, 0, 1);
                    glScaled(frontLegSize, 2 + 0.5, frontLegSize);
                    drawBox(1, 1, 1);
                glPopMatrix();

                if (VAL(LV_DETAIL) > 2) {

                    glTranslated(0, -2 - 0.5 + 0.2, 0);
                    glRotated(VAL(RIGHT_FRONT_ANGLE2), 0, 0, 1);

                    glPushMatrix();
                    glRotated(-180, 0, 0, 1);
                    glScaled(frontLegSize, 1.5, frontLegSize);
                    drawBox(1, 1, 1);
                    glPopMatrix();

                    if (VAL(LV_DETAIL) > 3) {

                        glTranslated(0, -1.5 + 0.2, 0);
                        glRotated(VAL(RIGHT_FRONT_ANGLE3), 0, 0, 1);

                        glPushMatrix();
                        glRotated(-180, 0, 0, 1);
                        glScaled(1, 0.3, frontLegSize);
                        drawBox(1, 1, 1);
                        glPopMatrix();
                    }
                }
            glPopMatrix();






            // front left leg
            frontLegZ = 1 - frontLegSize;

            // copied from above
            glPushMatrix();
                glTranslated(frontLegX, frontLegY + 0.5, frontLegZ);

                // rotate
                glRotated(VAL(LEFT_FRONT_ANGLE1), 0, 0, 1);

                glPushMatrix();
                    glRotated(-180, 0, 0, 1);
                    glScaled(frontLegSize, 2 + 0.5, frontLegSize);
                    drawBox(1, 1, 1);
                glPopMatrix();

                glTranslated(0, -2 - 0.5 + 0.2, 0);
                glRotated(VAL(LEFT_FRONT_ANGLE2), 0, 0, 1);

                if (VAL(LV_DETAIL) > 2) {
                    glPushMatrix();
                        glRotated(-180, 0, 0, 1);
                        glScaled(frontLegSize, 1.5, frontLegSize);
                        drawBox(1, 1, 1);
                    glPopMatrix();


                    glTranslated(0, -1.5 + 0.2, 0);
                    glRotated(VAL(LEFT_FRONT_ANGLE3), 0, 0, 1);

                    if (VAL(LV_DETAIL) > 3) {
                        glPushMatrix();
                            glRotated(-180, 0, 0, 1);
                            glScaled(1, 0.3, frontLegSize);
                            drawBox(1, 1, 1);
                        glPopMatrix();
                    }
                }
            glPopMatrix();






            // part5, 9, 12 back right leg

            double backLegSize = 0.6;
            double backLegX = 4.5;
            double backLegY = 1.7;
            double backLegZ = -1;

            double aux = 0.1;

            glPushMatrix();
            glTranslated(backLegX, backLegY + aux, backLegZ);
            // rotate
            glRotated(VAL(RIGHT_BACK_ANGLE1), 0, 0, 1);

            glPushMatrix();
            glRotated(-180, 0, 0, 1);
            glScaled(backLegSize, 2.5 + aux, backLegSize);
            drawBox(1, 1, 1);
            glPopMatrix();


            glTranslated(0, -2.5 + aux, 0);
            // rotate
            glRotated(VAL(RIGHT_BACK_ANGLE2), 0, 0, 1);

            if (VAL(LV_DETAIL) > 2) {

                glRotated(-30, 0, 0, 1);
                glPushMatrix();
                glRotated(-180, 0, 0, 1);
                glScaled(backLegSize, 3, backLegSize);
                drawBox(1, 1, 1);
                glPopMatrix();

                glTranslated(0, -3 + 3 * aux, 0);

                // rotate
                glRotated(VAL(RIGHT_BACK_ANGLE3), 0, 0, 1);
                glRotated(30, 0, 0, 1);

                if (VAL(LV_DETAIL) > 3) {

                    glPushMatrix();
                    glRotated(-180, 0, 0, 1);
                    glScaled(1, 0.3, backLegSize);
                    drawBox(1, 1, 1);
                    glPopMatrix();
                }
            }
            glPopMatrix();





            // left
            backLegZ = 1 - backLegSize;

            glPushMatrix();
            glTranslated(backLegX, backLegY + aux, backLegZ);
            // rotate
            glRotated(VAL(LEFT_BACK_ANGLE1), 0, 0, 1);

            glPushMatrix();
            glRotated(-180, 0, 0, 1);
            glScaled(backLegSize, 2.5 + aux, backLegSize);
            drawBox(1, 1, 1);
            glPopMatrix();


            glTranslated(0, -2.5 + aux, 0);
            // rotate
            glRotated(VAL(LEFT_BACK_ANGLE2), 0, 0, 1);

            if (VAL(LV_DETAIL) > 2) {
                glRotated(-30, 0, 0, 1);
                glPushMatrix();
                glRotated(-180, 0, 0, 1);
                glScaled(backLegSize, 3, backLegSize);
                drawBox(1, 1, 1);
                glPopMatrix();

                glTranslated(0, -3 + 3 * aux, 0);

                // rotate
                glRotated(VAL(LEFT_BACK_ANGLE3), 0, 0, 1);
                glRotated(30, 0, 0, 1);

                if (VAL(LV_DETAIL) > 3) {
                    glPushMatrix();
                    glRotated(-180, 0, 0, 1);
                    glScaled(1, 0.3, backLegSize);
                    drawBox(1, 1, 1);
                    glPopMatrix();
                }
            }
            glPopMatrix();



            // tail 
            glPushMatrix();
                glTranslated(4.5, 2, -0.1);
                glRotated(VAL(TAIL_ANGLE_X), 1, 0, 0);
                glRotated(VAL(TAIL_ANGLE_Y), 0, 1, 0);
                glRotated(-150 + VAL(TAIL_ANGLE_Z), 0, 0, 1);
                glScaled(0.2, 2.5, 0.2);
                drawBox(1, 1, 1);
            glPopMatrix();

        }
        glPopMatrix();
}


void DoggModel::drawAxis() {
    // draw axis
    glPushMatrix();
    glBegin(GL_LINES);

    // lines parallel to the axis
    //glColor3f(1, 0, 0);
    //glVertex3f(-50, -2, -20);
    //glVertex3f(50, -2, -20);
    //
    //glColor3f(0, 1, 0);
    //glVertex3f(0, -50, -20);
    //glVertex3f(0, 50, -20);
    //
    //glColor3f(0, 0, 1);
    //glVertex3f(0, -2, -100);
    //glVertex3f(0, -2, 10);

    // actual axis
    setDiffuseColor(1.0f, 0.0f, 0.0f); // red x
    glVertex3f(-5, 0, 0);
    glVertex3f(50, 0, 0);


    setDiffuseColor(0.0f, 1.0f, 0.0f); // green y
    glVertex3f(0, -5, 0);
    glVertex3f(0, 50, 0);

    setDiffuseColor(0.0f, 0.0f, 1.0f); // blue z
    glVertex3f(0, 0, -5);
    glVertex3f(0, 0, 50);

    glEnd();
    glPopMatrix();
    setDiffuseColor(1.0f, 1.0f, 1.0f);
}


void DoggModel::resetLeg() {
    SET(LEFT_FRONT_ANGLE1, 0);
    SET(LEFT_FRONT_ANGLE2, 0);
    SET(LEFT_FRONT_ANGLE3, 0);

    SET(RIGHT_FRONT_ANGLE1, 0);
    SET(RIGHT_FRONT_ANGLE2, 0);
    SET(RIGHT_FRONT_ANGLE3, 0);

    SET(LEFT_BACK_ANGLE1, 0);
    SET(LEFT_BACK_ANGLE2, 0);
    SET(LEFT_BACK_ANGLE3, 0);

    SET(RIGHT_BACK_ANGLE1, 0);
    SET(RIGHT_BACK_ANGLE2, 0);
    SET(RIGHT_BACK_ANGLE3, 0);

    SET(RESET_LEG, 0);
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

    controls[NECK_ANGLE] = ModelerControl("Neck Angle", -45, 45, 0.1f, 1);
	controls[HEAD_ANGLE_X] = ModelerControl("Head Angle in X direction", -45, 45, 0.1f, 0);
    controls[HEAD_ANGLE_Z] = ModelerControl("Head Angle in Z direction", 0, 45, 0.1f, 0);
    controls[LEFT_FRONT_ANGLE1] = ModelerControl("Left front upper  arm angle", -45, 45, 0.1f, 0);
    controls[LEFT_FRONT_ANGLE2] = ModelerControl("Left front middle arm angle", -45, 45, 0.1f, 0);
    controls[LEFT_FRONT_ANGLE3] = ModelerControl("Left front lower  arm angle", -45, 45, 0.1f, 0);
    controls[RIGHT_FRONT_ANGLE1] = ModelerControl("Right front upper  arm angle", -45, 45, 0.1f, 0);
    controls[RIGHT_FRONT_ANGLE2] = ModelerControl("Right front middle arm angle", -45, 45, 0.1f, 0);
    controls[RIGHT_FRONT_ANGLE3] = ModelerControl("Right front lower  arm angle", -45, 45, 0.1f, 0);
    controls[LEFT_BACK_ANGLE1] = ModelerControl("Left back upper  leg angle", -45, 45, 0.1f, 0);
    controls[LEFT_BACK_ANGLE2] = ModelerControl("Left back middle leg angle", -45, 45, 0.1f, 0);
    controls[LEFT_BACK_ANGLE3] = ModelerControl("Left back lower  leg angle", -45, 45, 0.1f, 0);
    controls[RIGHT_BACK_ANGLE1] = ModelerControl("Right back upper  leg angle", -45, 45, 0.1f, 0);
    controls[RIGHT_BACK_ANGLE2] = ModelerControl("Right back middle leg angle", -45, 45, 0.1f, 0);
    controls[RIGHT_BACK_ANGLE3] = ModelerControl("Right back lower  leg angle", -45, 45, 0.1f, 0);
    controls[TAIL_ANGLE_X] = ModelerControl("Tail Angle X", -45, 45, 0.1f, 0);
    controls[TAIL_ANGLE_Y] = ModelerControl("Tail Angle Y", -45, 45, 0.1f, 0);
    controls[TAIL_ANGLE_Z] = ModelerControl("Tail Angle Z", -15, 135, 0.1f, 0);

    controls[RESET_LEG] = ModelerControl("Reset Leg Pose", 0, 1, 1, 0);

    controls[LIGHT0_X] = ModelerControl("Light 0 X position", -5, 5, 0.1f, 4);
    controls[LIGHT0_Y] = ModelerControl("Light 0 Y position", -5, 5, 0.1f, 2);
    controls[LIGHT0_Z] = ModelerControl("Light 0 Z position", -5, 5, 0.1f, -4);
                                                              
    controls[LIGHT0_R] = ModelerControl("Light 0 red color", 0, 1, 0.01f, 1);
    controls[LIGHT0_G] = ModelerControl("Light 0 green color", 0, 1, 0.01f, 1);
    controls[LIGHT0_B] = ModelerControl("Light 0 blue color", 0, 1, 0.01f, 1);

    controls[DRAW_PROP] = ModelerControl("Draw propeller?", 0, 1, 1, 1);

    controls[LV_DETAIL] = ModelerControl("Level of Detail", 1, 4, 1, 1);

    ModelerApplication::Instance()->Init(&createDoggModel, controls, NUMCONTROLS);
    return ModelerApplication::Instance()->Run();
}
