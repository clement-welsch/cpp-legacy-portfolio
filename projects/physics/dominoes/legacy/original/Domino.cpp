#include "Domino.h"
#include <iostream>


using namespace std;

Domino::Domino()
{
        posX = posY = posZ = 0;
        dimX = dimY = dimZ = 0;
        orientation = 0;
        texture = 0;
}
Domino::Domino(float _posX,float _posY,float _posZ,float _dimX,float _dimY,float _dimZ,float _orientation,Texture * _texture)
{
        posX = _posX;
        posY = _posY;
        posZ = _posZ;
        dimX = _dimX;
        dimY = _dimY;
        dimZ = _dimZ;
        orientation = _orientation;
        texture = _texture;
}
Domino::~Domino()
{}
void Domino::afficher()
{
        float tmpdimX = dimX;
        float tmpdimY = dimY;
        float tmpdimZ = dimZ;
        
        tmpdimX /= 2.0f;
        tmpdimY /= 2.0f;
        tmpdimZ /= 2.0f;
        glPushMatrix();
        
        
        // Activation du mode d'utilisation de textures
        glEnable(GL_TEXTURE_2D);
        
        texture->utilise();
        
        //Création des six faces composant le Domino
		//---------------------------------------------------------
        // avant
        glNormal3f( 0.0f, 0.0f, 1.0f);
        glBegin(GL_QUADS);
                glTexCoord2f(0.5f,0.f);glVertex3f(-tmpdimX, tmpdimY, tmpdimZ);
                glTexCoord2f(0.5f,1.f);glVertex3f(-tmpdimX,-tmpdimY, tmpdimZ);
                glTexCoord2f(0.f,1.f);glVertex3f( tmpdimX,-tmpdimY, tmpdimZ);
                glTexCoord2f(0.f,0.f);glVertex3f( tmpdimX, tmpdimY, tmpdimZ);
        glEnd();
        // arrière
        glNormal3f( 0.0f, 0.0f, -1.0f);
        glBegin(GL_QUADS);
                glTexCoord2f(1.f,0.5f);glVertex3f( tmpdimX, tmpdimY, -tmpdimZ);
                glTexCoord2f(1.f,1.f);glVertex3f( tmpdimX,-tmpdimY, -tmpdimZ);
                glTexCoord2f(0.5f,1.f);glVertex3f(-tmpdimX,-tmpdimY, -tmpdimZ);
                glTexCoord2f(0.5f,0.5f);glVertex3f(-tmpdimX, tmpdimY, -tmpdimZ);
        glEnd();
        // Désactivation du mode d'utilisation de textures
        glDisable(GL_TEXTURE_2D);

        glColor3f(1.f,1.f,1.f);//les Faces dépourvu de texture ont une coloration Blanche
        // gauche
        glNormal3f( -1.0f, 0.0f, 0.0f);
        glBegin(GL_QUADS);
                glVertex3f(-tmpdimX, tmpdimY, -tmpdimZ);
                glVertex3f(-tmpdimX,-tmpdimY, -tmpdimZ);
                glVertex3f(-tmpdimX,-tmpdimY,  tmpdimZ);
                glVertex3f(-tmpdimX, tmpdimY,  tmpdimZ);
        glEnd();
        // droit
        glNormal3f( 1.0f, 0.0f, 0.0f);
        glBegin(GL_QUADS);
                glVertex3f(tmpdimX, tmpdimY,  tmpdimZ);
                glVertex3f(tmpdimX,-tmpdimY,  tmpdimZ);
                glVertex3f(tmpdimX,-tmpdimY, -tmpdimZ);
                glVertex3f(tmpdimX, tmpdimY, -tmpdimZ);
        glEnd();
        // Haut
        glNormal3f( 0.0f, 1.0f, 0.0f);
        glBegin(GL_QUADS);
                glVertex3f(-tmpdimX, tmpdimY,-tmpdimZ);
                glVertex3f(-tmpdimX, tmpdimY, tmpdimZ);
                glVertex3f( tmpdimX, tmpdimY,  tmpdimZ);
                glVertex3f( tmpdimX, tmpdimY, -tmpdimZ);
        glEnd();
        // Bas
        glNormal3f( 0.0f, -1.0f, 0.0f);
        glBegin(GL_QUADS);
                glVertex3f(-tmpdimX, -tmpdimY,-tmpdimZ);
                glVertex3f( tmpdimX, -tmpdimY,-tmpdimZ);
                glVertex3f( tmpdimX, -tmpdimY, tmpdimZ);
                glVertex3f(-tmpdimX, -tmpdimY, tmpdimZ);
        glEnd();
        glPopMatrix();

}
void Domino::DefinitPhysique(dWorldID world,dSpaceID space)
{
        dMass mass;
        body = dBodyCreate( world );
        dMassSetBoxTotal (&mass, 2, dimX,dimY,dimZ);
        dBodySetMass (body, &mass); 
        geom = dCreateBox(space,dimX,dimY,dimZ);
        dGeomSetBody (geom, body);
        dBodySetPosition(body,posX,posY,posZ);
        
        dMatrix3 R;
        
        dRFromAxisAndAngle(R, 0, 1, 0,orientation);
        dBodySetRotation(body, R);

}
