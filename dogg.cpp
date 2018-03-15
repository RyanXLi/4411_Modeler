#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include "camera.h"
#include <FL/gl.h>
#include "bitmap.h"
#include <math.h>
#include <vector>
#include "modelerglobals.h"

class camera;
// To make a DoggModel, we inherit off of ModelerView
class DoggModel : public ModelerView 
{
public:
    DoggModel(int x, int y, int w, int h, char *label) 
        : ModelerView(x,y,w,h,label) { }

    virtual void draw();
    void init();
	std::vector<char> gsentence(const int stage);
	void drawLsystem(const std::vector<char> & sentence);
    void drawAxis();
    void resetLeg();
	void frameAll(float dx, float dy, float dz);
    void animate(int index);
    void moveHead();
    void moveTail();
    bool checkLegParam();


    unsigned char * texImg;
    int texImgW = 256;
    int texImgH = 256;
    GLuint tex = 0;
    bool firstTime = 1;
    bool shouldMoveHead = 0;
    bool shouldMoveTail = 0;
    int headBoundary = 45;
    int tailBoundary = 45;
    int headBoundaryCount = 0;
    int tailBoundaryCount = 0;
    int headIncr = 1;
    int tailIncr = 1;
    bool hasDiffLegParam = 0;

    double anim_incr [12] = {
        5, 5, 5,
        -5, -5, -5,
        5, 5, 5,
        -5, -5, -5
    };

    double anim_min [12] = {
        -45, -45, -45,
        -45, -45, -45,
        -45, -45, -45,
        -45, -45, -45
    };

    double anim_max [12] = {
        45, 45, 45,
        45, 45, 45,
        45, 45, 45,
        45, 45, 45
    };

    double last_angle[12] = {
        0, 0, 0,
        0, 0, 0,
        0, 0, 0,
        0, 0, 0
    };
    
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

    if (firstTime) {
        init();
        firstTime = 0;
    }

    // This call takes care of a lot of the nasty projection 
    // matrix stuff.  Unless you want to fudge directly with the 
	// projection matrix, don't bother with this ...

	if (VAL(FRAME_ALL)) { 
		DoggModel::frameAll(VAL(XPOS), VAL(YPOS), VAL(ZPOS));
	}
	
	ModelerView::draw();

    if (VAL(RESET_LEG)) { DoggModel::resetLeg(); }

    hasDiffLegParam = checkLegParam();

	// draw the dogg model

	setAmbientColor(.1f,.1f,.1f);
	setDiffuseColor(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslated(VAL(XPOS), VAL(YPOS), VAL(ZPOS));

        // part 1: torso
		glPushMatrix();
		    glTranslated(-2, 0, -1);
            glScaled(2, 3, 2);
		    drawTextureBox(1,1,1);
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
            drawTextureBox(1, 1, 1);
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
                drawTextureBox(1, 1, 1);
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
                drawTextureBox(1, 1, 1);
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
            drawTextureBox(1, 1, 1);
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

                if (VAL(CHEERFULNESS) < 0 && hasDiffLegParam) {
                    shouldMoveHead = 1;                    
                }
                moveHead();

                glPushMatrix();
                glScaled(0.75, 3, 2);
                drawTextureBox(1, 1, 1);
                glPopMatrix();

                glTranslated(0.75, 0, 0);

                glPushMatrix();
                glScaled(0.75, 2, 2);
                drawTextureBox(1, 1, 1);
                glPopMatrix();

                // torus
                //drawAxis();

                glPushMatrix();
                glTranslated(1.5, 1, 1);
                glRotated(90, 0, 1, 0);
                drawTorus(0.8, 0.2);
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
                animate(RIGHT_FRONT_ANGLE1);

                glPushMatrix();
                    glRotated(-180, 0, 0, 1);
                    glScaled(frontLegSize, 2 + 0.5, frontLegSize);
                    drawTextureBox(1, 1, 1);
                glPopMatrix();

                if (VAL(LV_DETAIL) > 2) {

                    glTranslated(0, -2 - 0.5 + 0.2, 0);
                    glRotated(VAL(RIGHT_FRONT_ANGLE2), 0, 0, 1);
                    animate(RIGHT_FRONT_ANGLE2);

                    glPushMatrix();
                    glRotated(-180, 0, 0, 1);
                    glScaled(frontLegSize, 1.5, frontLegSize);
                    drawTextureBox(1, 1, 1);
                    glPopMatrix();

                    if (VAL(LV_DETAIL) > 3) {

                        glTranslated(0, -1.5 + 0.2, 0);
                        glRotated(VAL(RIGHT_FRONT_ANGLE3), 0, 0, 1);
                        animate(RIGHT_FRONT_ANGLE3);

                        glPushMatrix();
                        glRotated(-180, 0, 0, 1);
                        glScaled(1, 0.3, frontLegSize);
                        drawTextureBox(1, 1, 1);
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
                animate(LEFT_FRONT_ANGLE1);

                glPushMatrix();
                    glRotated(-180, 0, 0, 1);
                    glScaled(frontLegSize, 2 + 0.5, frontLegSize);
                    drawTextureBox(1, 1, 1);
                glPopMatrix();

                glTranslated(0, -2 - 0.5 + 0.2, 0);
                glRotated(VAL(LEFT_FRONT_ANGLE2), 0, 0, 1);
                animate(LEFT_FRONT_ANGLE2);

                if (VAL(LV_DETAIL) > 2) {
                    glPushMatrix();
                        glRotated(-180, 0, 0, 1);
                        glScaled(frontLegSize, 1.5, frontLegSize);
                        drawTextureBox(1, 1, 1);
                    glPopMatrix();


                    glTranslated(0, -1.5 + 0.2, 0);
                    glRotated(VAL(LEFT_FRONT_ANGLE3), 0, 0, 1);
                    animate(LEFT_FRONT_ANGLE3);

                    if (VAL(LV_DETAIL) > 3) {
                        glPushMatrix();
                            glRotated(-180, 0, 0, 1);
                            glScaled(1, 0.3, frontLegSize);
                            drawTextureBox(1, 1, 1);
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
            animate(RIGHT_BACK_ANGLE1);

            glPushMatrix();
            glRotated(-180, 0, 0, 1);
            glScaled(backLegSize, 2.5 + aux, backLegSize);
            drawTextureBox(1, 1, 1);
            glPopMatrix();


            glTranslated(0, -2.5 + aux, 0);
            // rotate
            glRotated(VAL(RIGHT_BACK_ANGLE2), 0, 0, 1);
            animate(RIGHT_BACK_ANGLE2);

            if (VAL(LV_DETAIL) > 2) {

                glRotated(-30, 0, 0, 1);
                glPushMatrix();
                glRotated(-180, 0, 0, 1);
                glScaled(backLegSize, 3, backLegSize);
                drawTextureBox(1, 1, 1);
                glPopMatrix();

                glTranslated(0, -3 + 3 * aux, 0);

                // rotate
                glRotated(VAL(RIGHT_BACK_ANGLE3), 0, 0, 1);
                animate(RIGHT_BACK_ANGLE3);
                glRotated(30, 0, 0, 1);

                if (VAL(LV_DETAIL) > 3) {

                    glPushMatrix();
                    glRotated(-180, 0, 0, 1);
                    glScaled(1, 0.3, backLegSize);
                    drawTextureBox(1, 1, 1);
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
            animate(LEFT_BACK_ANGLE1);

            glPushMatrix();
            glRotated(-180, 0, 0, 1);
            glScaled(backLegSize, 2.5 + aux, backLegSize);
            drawTextureBox(1, 1, 1);
            glPopMatrix();


            glTranslated(0, -2.5 + aux, 0);
            // rotate
            glRotated(VAL(LEFT_BACK_ANGLE2), 0, 0, 1);
            animate(LEFT_BACK_ANGLE2);

            if (VAL(LV_DETAIL) > 2) {
                glRotated(-30, 0, 0, 1);
                glPushMatrix();
                glRotated(-180, 0, 0, 1);
                glScaled(backLegSize, 3, backLegSize);
                drawTextureBox(1, 1, 1);
                glPopMatrix();

                glTranslated(0, -3 + 3 * aux, 0);

                // rotate
                glRotated(VAL(LEFT_BACK_ANGLE3), 0, 0, 1);
                animate(LEFT_BACK_ANGLE3);
                glRotated(30, 0, 0, 1);

                if (VAL(LV_DETAIL) > 3) {
                    glPushMatrix();
                    glRotated(-180, 0, 0, 1);
                    glScaled(1, 0.3, backLegSize);
                    drawTextureBox(1, 1, 1);
                    glPopMatrix();
                }
            }
            glPopMatrix();



            // tail 
            glPushMatrix();
                glTranslated(4.5, 2, -0.1);
                glRotated(VAL(TAIL_ANGLE_X), 1, 0, 0);
                glRotated(VAL(TAIL_ANGLE_Y), 0, 1, 0);

                if (VAL(CHEERFULNESS) > 0 && hasDiffLegParam) {
                    shouldMoveTail = 1;                    
                }
                moveTail();

                glRotated(-150 + VAL(TAIL_ANGLE_Z), 0, 0, 1);
                glScaled(0.2, 2.5, 0.2);
                drawTextureBox(1, 1, 1);
            glPopMatrix();

        }
		
		if (VAL(LSYSTEM_SWITCH)) {
			std::vector<char> sen = gsentence(VAL(LSYSTEM_STAGE));
			glPushMatrix();
				drawLsystem(sen);
			glPopMatrix();
		}

	glPopMatrix();

}


bool DoggModel::checkLegParam() {
    bool ret = FALSE;
    for (int i = 0; i < 12; i++) {
        if (VAL(LEFT_FRONT_ANGLE1 + i) != last_angle[i]) {
            ret = TRUE;
            last_angle[i] = VAL(LEFT_FRONT_ANGLE1 + i);
        }
    }
    return ret;
}

void DoggModel::moveHead() {
    if (!shouldMoveHead) {
        return;
    }
    if (headBoundaryCount >= 2) {
        shouldMoveHead = 0;
        headBoundaryCount = 0;
    }
    if (VAL(HEAD_ANGLE_X) + headIncr >= headBoundary 
        || VAL(HEAD_ANGLE_X) + headIncr <= -headBoundary) {
        headBoundaryCount++;
        headIncr = -headIncr;
    }
    SET(HEAD_ANGLE_X, VAL(HEAD_ANGLE_X) + headIncr);
}

void DoggModel::moveTail() {
    if (!shouldMoveTail) {
        return;
    }
    if (tailBoundaryCount >= 2) {
        shouldMoveTail = 0;
        tailBoundaryCount = 0;
    }
    if (VAL(TAIL_ANGLE_Y) + tailIncr >= tailBoundary
        || VAL(TAIL_ANGLE_Y) + tailIncr <= -tailBoundary) {
        tailBoundaryCount++;
        tailIncr = -tailIncr;
    }
    SET(TAIL_ANGLE_Y, VAL(TAIL_ANGLE_Y) + tailIncr);
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

void DoggModel::frameAll(float dx, float dy, float dz)
{
	m_camera->frameAll(dx,dy,dz);
	//gluLookAt(3.119846,3.837214,24.113871,2.040295,0.852658,-0.159513,0,1,0);
	SET(FRAME_ALL, 0);
}

void DoggModel::init() {
    //texImg = Fl_Shared_Image::get("texImg.png", 256, 256);
    texImg = readBMP("texImg.bmp", texImgW, texImgH);
    if (texImg == nullptr) {printf("Texture image not found.\n");}

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //float pixels[] = {
    //    0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
    //    0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f
    //};
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texImgW, texImgH, 0, GL_RGB, GL_UNSIGNED_BYTE, texImg);

}

std::vector<char> DoggModel::gsentence(const int stage)
{
	std::vector<char> current, next;
	current.push_back('F');
	for (int i = 0; i < stage; ++i) {
		next.clear();
		for (int l = 0; l < current.size(); ++l) {
			// F -¡ú FF+[+F-F-F]-[-F+F+F]
			if (current[l] == 'F') {
				next.push_back('F');
				next.push_back('F');
				next.push_back('+');
				next.push_back('[');
				next.push_back('+');
				next.push_back('F');
				next.push_back('-');
				next.push_back('F');
				next.push_back('-');
				next.push_back('F');
				next.push_back(']');
				next.push_back('-');
				next.push_back('[');
				next.push_back('-');
				next.push_back('F');
				next.push_back('+');
				next.push_back('F');
				next.push_back('+');
				next.push_back('F');
				next.push_back(']');
			}
			else next.push_back(current[l]);
		}
		current = next;
	}
	//for (int i = 0; i < current.size(); ++i) printf("%c", current[i]);
	//printf("\n");
	return current;
}

void DoggModel::drawLsystem(const std::vector<char>& sentence)
{
	float len = 0.2;
	glTranslated(1, 2.5, 0);
	for (int i = 0; i < sentence.size(); ++i) {
		char c = sentence[i];
		switch (c) {
		case 'F':
			glLineWidth(3.0f);
			glBegin(GL_LINES);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, len, 0.0f);
			glEnd();
			glTranslatef(0.0f, len, 0.0f);
			break;
		case '+':
			glRotatef(-25, 0, 0, 1);
			break;
		case '-':
			glRotatef(25, 0, 0, 1);
			break;
		case '[':
			glPushMatrix();
			break;
		case ']':
			glPopMatrix();
			break;
		default: break;
		}
	}
}

void DoggModel::animate(int index) {

    if (ModelerApplication::Instance()->m_animating) {
        int anim_part_count = index - LEFT_FRONT_ANGLE1;

        if (VAL(index) >= anim_max[anim_part_count] || VAL(index) <= anim_min[anim_part_count]) {
            anim_incr[anim_part_count] = - anim_incr[anim_part_count];
        }
        SET(index, VAL(index) + anim_incr[anim_part_count]);
    }
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
	controls[FRAME_ALL] = ModelerControl("Frame All", 0, 1, 1, 0);

    controls[LIGHT0_X] = ModelerControl("Light 0 X position", -5, 5, 0.1f, 4);
    controls[LIGHT0_Y] = ModelerControl("Light 0 Y position", -5, 5, 0.1f, 2);
    controls[LIGHT0_Z] = ModelerControl("Light 0 Z position", -5, 5, 0.1f, -4);
                                                              
    controls[LIGHT0_R] = ModelerControl("Light 0 red color", 0, 1, 0.01f, 1);
    controls[LIGHT0_G] = ModelerControl("Light 0 green color", 0, 1, 0.01f, 1);
    controls[LIGHT0_B] = ModelerControl("Light 0 blue color", 0, 1, 0.01f, 1);

    controls[DRAW_PROP] = ModelerControl("Draw propeller?", 0, 1, 1, 1);

    controls[LV_DETAIL] = ModelerControl("Level of Detail", 1, 4, 1, 4);

    controls[APPLY_TEX] = ModelerControl("Apply Texture", 0, 1, 1, 0);

	controls[LSYSTEM_SWITCH] = ModelerControl("Display Tree", 0, 1, 1, 0);
	controls[LSYSTEM_STAGE] = ModelerControl("LSystem Stage", 1, 6, 1, 3);

    controls[CHEERFULNESS] = ModelerControl("cheerfulness level", -1, 1, 1, 0);

    ModelerApplication::Instance()->Init(&createDoggModel, controls, NUMCONTROLS);

    return ModelerApplication::Instance()->Run();
}
