#include "MaFenetre.h"
#include <iostream>
#include <fstream>

using namespace std;

MaFenetre::MaFenetre( ) : QGLWidget()
{
}
MaFenetre::MaFenetre(Mesh *m) : QGLWidget()
{
    mesh = m;
    mesh2 = new Mesh();
    mesh3 = new Mesh();
    mesh4 = new Mesh();
    mesh5 = new Mesh();
    mesh6 = new Mesh();
}
Vert MaFenetre::SommetBord (Vert v)
{
    bool bBord = false;
    double x,y,z;
    Vert * Temp = new Vert();

    for (unsigned int i=0; i<v.voisin.size(); i++)
        if(v.voisin.at(i)->paire->f==NULL)
            bBord = true;
    if(bBord == true)
    {
        //On recalcule v
        x = (v.pos[0] * 3)/4;
        y = (v.pos[1] * 3)/4;
        z = (v.pos[2] * 3)/4;
        for (unsigned int i=0; i<v.voisin.size(); i++)
        {
            Temp = v.voisin.at(i)->queue;
            if (Temp->id == v.id)
                Temp = v.voisin.at(i)->tete;
            x += Temp->pos[0]/8;
            y += Temp->pos[1]/8;
            z += Temp->pos[2]/8;
        }
        v.pos[0] = x;
        v.pos[1] = y;
        v.pos[2] = z;
    }
    return v;
}
void MaFenetre::Loop (Mesh *m, Mesh *m2)
{
    unsigned int vHEsize = m->vHEdges.size();
    unsigned int vVertsize = m->vVerts.size();
    unsigned int iNbVerts = 0;
    double x,y,z;
    //---------------------------------------------------
    // Création et Ajout des nouveaux points
    //---------------------------------------------------
    for (unsigned int j = 0; j < vHEsize; j++)
        m->vHEdges.at(j)->nouveau = NULL;
    for (unsigned int j = 0; j < vHEsize; j++)
    {
        if (m->vHEdges.at(j)->nouveau == NULL)
        {
            Vert P1;
            Vert P2;
            Vert * Temp = new Vert();

            P1 = *m->vHEdges.at(j)->queue;
            P2 = *m->vHEdges.at(j)->tete;
            //Si on est sur une arête de bord
            if(m->vHEdges.at(j)->paire->f == NULL)
            {
                //On recalcule P1 & P2
                P1 = SommetBord (P1);
                P2 = SommetBord (P2);

                //On recalcule le nouveau sommet
                x = P1.pos[0]/2;
                x += P2.pos[0]/2;

                y = P1.pos[1]/2;
                y += P2.pos[1]/2;

                z = P1.pos[2]/2;
                z += P2.pos[2]/2;

                //On trouve le nouveau Sommet
                Temp->pos[0] = x;
                Temp->pos[1] = y;
                Temp->pos[2] = z;

                m->vHEdges.at(j)->nouveau = Temp;
            }
            //Si ce n'est pas une arête du bord
            else
            {
                Vert P3;
                Vert P4;
                unsigned int iHE;
                //On trouve P3 et P4
                for (unsigned int k=0; k<m->vFaces.size(); k++)
                {
                    if (m->vFaces.at(k)->he->tete->id == P1.id)
                    {
                        iHE = m->vFaces.at(k)->he->id;
                        P3 = *m->vFaces.at(k)->he->queue;
                        P4 = *m->vFaces.at(k)->he->suivante->paire->suivante->tete;
                    }
                    else if (m->vFaces.at(k)->he->queue->id == P1.id)
                    {
                        iHE = m->vFaces.at(k)->he->suivante->id;
                        P3 = *m->vFaces.at(k)->he->suivante->tete;
                        P4 = *m->vFaces.at(k)->he->paire->suivante->tete;
                    }
                    else if (m->vFaces.at(k)->he->queue->id == P2.id)
                    {
                        iHE = m->vFaces.at(k)->he->id;
                        P3 = *m->vFaces.at(k)->he->tete;
                        P4 = *m->vFaces.at(k)->he->precedente->paire->suivante->tete;
                    }
                }
                //On teste si les sommets sont des sommets de bord
                P1 = SommetBord (P1);
                P2 = SommetBord (P2);
                P3 = SommetBord (P3);
                P4 = SommetBord (P4);

                //On recalcule le nouveau sommet
                x = (P1.pos[0] * 3)/8;
                x += (P2.pos[0] * 3)/8;
                x += P3.pos[0]/8;
                x += P4.pos[0]/8;

                y = (P1.pos[1] * 3)/8;
                y += (P2.pos[1] * 3)/8;
                y += P3.pos[1]/8;
                y += P4.pos[1]/8;

                z = (P1.pos[2] * 3)/8;
                z += (P2.pos[2] * 3)/8;
                z += P3.pos[2]/8;
                z += P4.pos[2]/8;

                //On trouve le nouveau Sommet
                Temp->pos[0] = x;
                Temp->pos[1] = y;
                Temp->pos[2] = z;

                m->vHEdges.at(j)->nouveau = Temp;
            }
            //On copie le nouveau point dans la paire
            m->vHEdges.at(j)->paire->nouveau = m->vHEdges.at(j)->nouveau;
            m2->vVerts.push_back(m->vHEdges.at(j)->nouveau);
            iNbVerts++;
        }
    }
    //---------------------------------------------------
    // Ajout des points existants
    //---------------------------------------------------
    for (unsigned int i = 0; i < vVertsize; i++)
    {
        Vert Temp;
        //Créer le nouveau sommet s->nouveau
        m->vVerts.at(i)->nouveau = new Vert();

        Temp.pos[0] = m->vVerts.at(i)->pos[0];
        Temp.pos[1] = m->vVerts.at(i)->pos[1];
        Temp.pos[2] = m->vVerts.at(i)->pos[2];
        //Calculer la position de s->nouveau
        Temp = SommetBord(Temp);

        m->vVerts.at(i)->nouveau->pos[0] = Temp.pos[0];
        m->vVerts.at(i)->nouveau->pos[1] = Temp.pos[1];
        m->vVerts.at(i)->nouveau->pos[2] = Temp.pos[2];
        //Ajouter s->nouveau dans Mesh2
        m2->vVerts.push_back(m->vVerts.at(i)->nouveau);
        iNbVerts++;
    }
    unsigned int vFacesize = m->vFaces.size();
    unsigned int iNbEdges = 0;
    unsigned int iNbFaces = 0;

    //---------------------------------------------------
    // Ajout des Faces et arêtes
    //---------------------------------------------------
    for (unsigned int k = 0; k < vFacesize; k++)
    {
        vector <Vert *> vP;

        vP.push_back(m->vFaces.at(k)->he->queue->nouveau);//P1
        vP.push_back(m->vFaces.at(k)->he->nouveau);//NP1
        vP.push_back(m->vFaces.at(k)->he->precedente->nouveau);//NP3
        vP.push_back(m->vFaces.at(k)->he->nouveau);//NP1
        vP.push_back(m->vFaces.at(k)->he->tete->nouveau);//P2
        vP.push_back(m->vFaces.at(k)->he->suivante->nouveau);//NP2
        vP.push_back(m->vFaces.at(k)->he->suivante->nouveau);//NP2
        vP.push_back(m->vFaces.at(k)->he->suivante->tete->nouveau);//P3
        vP.push_back(m->vFaces.at(k)->he->precedente->nouveau);//NP3
        vP.push_back(m->vFaces.at(k)->he->nouveau);//NP1
        vP.push_back(m->vFaces.at(k)->he->suivante->nouveau);//NP2
        vP.push_back(m->vFaces.at(k)->he->precedente->nouveau);//NP3

        for (unsigned int i = 0; i< vP.size(); i = i +3)
        {
            Face * F = new Face();
            Halfedge * HE = new Halfedge();
            Halfedge * HE2 = new Halfedge();
            Halfedge * HE3 = new Halfedge();

            //F
            F->id =iNbFaces;
            iNbFaces++;
            F->nouveau = NULL;
            F->he = HE;
            //HE
            HE->id = iNbEdges;
            iNbEdges++;
            HE->queue = vP.at(i);
            HE->tete= vP.at(i+1);
            HE->nouveau = NULL;
            HE->paire = NULL;
            HE->precedente = HE3;
            HE->suivante = HE2;
            HE->f = F;
            //HE2
            HE2->id = iNbEdges;
            iNbEdges++;
            HE2->queue = vP.at(i+1);
            HE2->tete= vP.at(i+2);
            HE2->nouveau = NULL;
            HE2->paire = NULL;
            HE2->precedente = HE;
            HE2->suivante = HE3;
            HE2->f = F;
            //HE3
            HE3->id = iNbEdges;
            iNbEdges++;
            HE3->queue = vP.at(i+2);
            HE3->tete= vP.at(i);
            HE3->nouveau = NULL;
            HE3->paire = NULL;
            HE3->precedente = HE2;
            HE3->suivante = HE;
            HE3->f = F;

            //Ajout
            m2->vFaces.push_back(F);
            m2->vHEdges.push_back(HE);
            m2->vHEdges.push_back(HE2);
            m2->vHEdges.push_back(HE3);

        }
        //HE3 - HE9
        m2->vHEdges.at(iNbEdges-10)->paire = m2->vHEdges.at(iNbEdges-4);
        m2->vHEdges.at(iNbEdges-4)->paire = m2->vHEdges.at(iNbEdges-10);
        //HE6 - HE7
        m2->vHEdges.at(iNbEdges-7)->paire = m2->vHEdges.at(iNbEdges-6);
        m2->vHEdges.at(iNbEdges-6)->paire = m2->vHEdges.at(iNbEdges-7);
        //HE3 - HE9
        m2->vHEdges.at(iNbEdges-5)->paire = m2->vHEdges.at(iNbEdges-1);
        m2->vHEdges.at(iNbEdges-1)->paire = m2->vHEdges.at(iNbEdges-5);

    }
    m2->nbVerts = iNbVerts;
    m2->nbEdges = iNbEdges;
    m2->nbFaces = iNbFaces;
    /*std::cout<<"----FIN---- "<<std::endl;
    std::cout<<m->nbVerts<<" Verts for mesh vs mesh2 Verts : "<<m2->vVerts.size()<<std::endl;
    std::cout<<m->nbEdges<<" HE for mesh vs mesh2 HE : "<<m2->nbEdges<<std::endl;
    std::cout<<m->nbFaces<<" Faces for mesh vs mesh2 Faces : "<<m2->nbFaces<<std::endl;*/
    VertVoisin (m2);//Recherche les arètes voisines
    HEPaire (m2);//Recherche les arètes paires
    VertNormal (m2);//Calcul des normales
    Looptroop++;
}
void MaFenetre::Repere()
{
        glDisable(GL_LIGHTING);
        glBegin(GL_LINES);
        /*X*/
        glColor3f(0.0, 0.0, 1.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(10.0, 0.0, 0.0);
        /*Y*/
        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 10.0, 0.0);
        /*Z*/
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 0.0, 10.0);
        glEnd();
        glEnable(GL_LIGHTING);
}
void MaFenetre::AffichageVerts(Mesh * m)
{
    glColor3f (0.0, 0.0, 1.0);
    glPointSize(4.0);
    glBegin(GL_POINTS);
    for (int i = 0; i < m->nbVerts; i++)
        glVertex3f(m->vVerts[i]->pos[0], m->vVerts[i]->pos[1], m->vVerts[i]->pos[2]);
    glEnd();
}
void MaFenetre::AffichageNouveaux(Mesh * m)
{
    glColor3f (0.2, 0.8, 0.8);
    glPointSize(4.0);
    for (unsigned int j = 0; j < m->vHEdges.size();j++)
    {
        if (m->vHEdges[j]->nouveau!=NULL)
        {
            glBegin(GL_POINTS);
                glVertex3f(m->vHEdges[j]->nouveau->pos[0], m->vHEdges[j]->nouveau->pos[1], m->vHEdges[j]->nouveau->pos[2]);
            glEnd();
        }
    }
}
void MaFenetre::AffichageHE(Mesh * m, int i)
{
    if (i==1)
        glColor3f (1.0, 0.0, 0.0);
    else
        glColor3f (0.0, 0.0, 1.0);

    glPointSize(4.0);
    for (unsigned int j = 0; j < m->vHEdges.size();j++)
    {
        glBegin(GL_LINES);
            glVertex3f(m->vHEdges[j]->tete->pos[0], m->vHEdges[j]->tete->pos[1], m->vHEdges[j]->tete->pos[2]);
            glVertex3f(m->vHEdges[j]->queue->pos[0], m->vHEdges[j]->queue->pos[1], m->vHEdges[j]->queue->pos[2]);
        glEnd();
    }
}
void MaFenetre::AffichageFaces(Mesh * m)
{
    glColor3f (0.0, 0.0, 1.0);
    glPointSize(4.0);
    std::vector <Vert*> vVertAffichage;

    for (unsigned int k = 0; k < m->vFaces.size();k++)
    {
        vVertAffichage.push_back(m->vFaces[k]->he->queue);
        vVertAffichage.push_back(m->vFaces[k]->he->tete);
        vVertAffichage.push_back(m->vFaces[k]->he->suivante->tete);

        glBegin(GL_TRIANGLES);
        glNormal3d(vVertAffichage[0]->normal[0], vVertAffichage[0]->normal[1], vVertAffichage[0]->normal[2]);
        glVertex3d(vVertAffichage[0]->pos[0], vVertAffichage[0]->pos[1], vVertAffichage[0]->pos[2]);
        glNormal3d(vVertAffichage[1]->normal[0], vVertAffichage[0]->normal[1], vVertAffichage[1]->normal[2]);
        glVertex3d(vVertAffichage[1]->pos[0], vVertAffichage[1]->pos[1], vVertAffichage[1]->pos[2]);
        glNormal3d(vVertAffichage[2]->normal[0], vVertAffichage[2]->normal[1], vVertAffichage[2]->normal[2]);
        glVertex3d(vVertAffichage[2]->pos[0], vVertAffichage[2]->pos[1], vVertAffichage[2]->pos[2]);
        glEnd();
        vVertAffichage.clear();
    }
}
void MaFenetre::Affichage(Mesh * _m)
{
    //Vert
    for (unsigned int i = 0; i < _m->vVerts.size(); i++)
    {
        std::cout<<"V ("<<i<<") "<<_m->vVerts.at(i)->pos[0]<<" "<<_m->vVerts.at(i)->pos[1]<<" "<<_m->vVerts.at(i)->pos[2]<<std::endl;
    }
    std::cout<<"----------------"<<std::endl;
    //HE
    for (unsigned int j = 0; j < _m->vHEdges.size(); j++)
    {
        std::cout<<"HE ("<<j<<") "<<_m->vHEdges.at(j)->queue->id<<" "<<_m->vHEdges.at(j)->tete->id;
        std::cout<<" pre("<<_m->vHEdges.at(j)->precedente->id<<") suiv("<<_m->vHEdges.at(j)->suivante->id<<")"<<std::endl;
    }
    std::cout<<"----------------"<<std::endl;
    std::cout<<"----------------"<<std::endl;
    //Face
    /*unsigned int FVertID;
    unsigned int FHEID;
    unsigned int HEVertID;
    unsigned int HEID;
    for (unsigned int k = 0; k <_m->vFaces.size(); k++)
    {
        std::cout<<"F ("<<k<<")"<<std::endl;

        //HEs
        FHEID = _m->vFaces.at(k)->he->id;
        std::cout<<"HE ("<<FHEID<<") ";

        HEVertID = _m->vFaces.at(k)->he->suivante->id;
        //On parcours les arètes de la face
        //Si VID == FVertID alors on a parcourut toutes les arètes.
        while (HEVertID != FHEID)
        {
            std::cout<<"| HE ("<<HEVertID<<") ";
            HEVertID = _m->vHEdges.at(HEVertID)->suivante->id;
        }
        std::cout<<std::endl;
        //Verts
        FVertID = _m->vFaces.at(k)->he->queue->id;
        std::cout<<FVertID<<" "<<_m->vFaces.at(k)->he->tete->id<<" ";

        HEID = _m->vFaces.at(k)->he->suivante->id;
        HEVertID = _m->vHEdges.at(HEID)->tete->id;
        //On parcours les arètes de la face
        //Si VID == FVertID alors on a parcourut toutes les arètes.
        while (HEVertID != FVertID)
        {
            std::cout<<HEVertID<<" ";
            HEID = _m->vHEdges.at(HEID)->suivante->id;
            HEVertID = _m->vHEdges.at(HEID)->tete->id;
        }
        std::cout<<std::endl;
    }
    std::cout<<"----------------"<<std::endl;
    std::cout<<"----------------"<<std::endl;
    std::cout<<"----------------"<<std::endl;*/
}
void MaFenetre::initializeGL()
{
        glEnable(GL_DEPTH_TEST);// Active le test de profondeur

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 100.0);
	
	glClearColor(0.8, 0.8, 0.7, 1.0);
	glShadeModel(GL_FLAT);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        glEnable(GL_CULL_FACE);

        glEnable(GL_LIGHTING);	// Active l'éclairage
        glEnable(GL_LIGHT0);	// Allume la lumière n°1

        //Tracball
        translationX = 0.0;
        translationY = 0.0;
        translationZ = -200.0;

        this->eloign = -2.0;

        gluLookAt(translationX, translationY, -translationZ, rotationY, -rotationX, rotationZ+1.0, 0, 1, 0);

        //Loop
        Looptroop = 0;
}
void MaFenetre::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)width/(GLfloat)height, 0.1, 128.0);

	glMatrixMode(GL_MODELVIEW);
}
void MaFenetre::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    double baryx = 0;
	double baryy = 0;
	double baryz = 0;

	for (int i = 0; i < mesh->nbVerts; i++)
    {
        baryx = baryx + mesh->vVerts[i]->pos[0];
        baryy = baryy + mesh->vVerts[i]->pos[1];
        baryz = baryz + mesh->vVerts[i]->pos[2];
    }

	baryx = baryx / (mesh->nbVerts - 1.0);
	baryy = baryy / (mesh->nbVerts - 1.0);
    baryz = baryz / (mesh->nbVerts - 1.0);

    glTranslatef(0.0f, 0.0f, this->eloign);

    glRotatef(this->rotationX, 1.0, 0.0, 0.0);
    glRotatef(this->rotationY, 0.0, 1.0, 0.0);
    glRotatef(this->rotationZ, 0.0, 0.0, 1.0);

    glPushMatrix();
        Repere();
        //AffichageVerts(mesh);
        //AffichageHE(mesh);
        //AffichageFaces(mesh);
        //--------------------------------------------------------------------------------------------------
        if((Looptroop < 1)&&(mesh->nbEdges>0))
        {
            Loop (mesh, mesh2);
            Loop (mesh2, mesh3);
            /*Loop (mesh3, mesh4);
            Loop (mesh4, mesh5);
            Loop (mesh5, mesh6);*/
        }
        AffichageHE(mesh,1);
        AffichageHE(mesh2,2);
        //AffichageNouveaux(mesh);
        //AffichageHE(mesh2);
        //--------------------------------------------------------------------------------------------------
    glPopMatrix();
}
void MaFenetre::mousePressEvent(QMouseEvent *event)
{
    this->lastPos = event->pos();
}
void MaFenetre::mouseMoveEvent(QMouseEvent *event)
{
    GLfloat dx = GLfloat(event->x()-this->lastPos.x())/this->width();
    GLfloat dy = GLfloat(event->y()-this->lastPos.y())/this->height();

    if(event->buttons() & Qt::LeftButton)
    {
        this->rotationX += 180*dy;
        this->rotationY += 180*dx;
        this->rotationZ = 0;
        this->updateGL();
    }
    else if (event->buttons() & Qt::RightButton)
    {
        this->rotationX += 180*dy;
        this->rotationY = 0;
        this->rotationZ += 180*dx;
        this->updateGL();
    }
    else if (event->buttons() & Qt::MidButton)
    {
        GLfloat d = GLfloat(this->lastPos.y()) - event->y();
        if (d > 0)
        {
            std::cout<<"ICI"<<std::endl;
            this->eloign += 0.25;
        }
        else if (d < 0)
        {
            this->eloign -= 0.25;
        }
    }
    this->lastPos = event->pos();
}
void MaFenetre::VertVoisin (Mesh* _m)
{
    unsigned int iVsize = _m->vVerts.size();//Taille du vector Vert
    unsigned int iHEsize = _m->vHEdges.size();//Taille du vector HE
    unsigned int iVertId;//Id du sommet/Vert
    unsigned int iHETete;//Id de la tête de l'arète
    unsigned int iHEQueue;//Id de la queue de l'arète

    for (unsigned int i = 0; i < iVsize; i++)
    {
        iVertId = _m->vVerts.at(i)->id;
        for (unsigned int j = 0; j < iHEsize; j++)
        {
            iHETete =  _m->vHEdges.at(j)->tete->id;
            iHEQueue = _m->vHEdges.at(j)->queue->id;
            if((iVertId == iHETete)||(iVertId == iHEQueue))
                _m->vVerts.at(i)->voisin.push_back (_m->vHEdges.at(j));
        }
    }
}
void MaFenetre::HEPaire(Mesh * _m)
{
    unsigned int iHEsize = _m->vHEdges.size(); //Taille du vector HE
    unsigned int iFsize = _m->vFaces.size(); //Taille du vector Face
    unsigned int iHETete;//Id de la tête de la première arète
    unsigned int iHEQueue;//Id de la queue de la première arète
    unsigned int iHEID;//Id de la première arète
    unsigned int iHE2Tete;//Id de la tête de la seconde arète
    unsigned int iHE2Queue;//Id de la queue de la seconde arète
    unsigned int iHE2ID;//Id de la seconde arète
    unsigned int iFHE;//Id de l'aèrete lié à la face
    unsigned int iVoisinSize; //Taille du Vector voisin
    Halfedge * he = new Halfedge ();
    bool bTest = false;
    for (unsigned int i = 0; i < iHEsize; i++)
        _m->vHEdges.at(i)->paire = NULL;
    for (unsigned int i = 0; i < iHEsize; i++)
    {
        if (_m->vHEdges.at(i)->paire == NULL)
        {
            iHEID = _m->vHEdges.at(i)->id;
            iHETete = _m->vHEdges.at(i)->tete->id;
            iHEQueue = _m->vHEdges.at(i)->queue->id;
            iVoisinSize = _m->vHEdges.at(i)->tete->voisin.size();
            for (unsigned int j = 0; j < iVoisinSize; j++)
            {
                iHE2ID = _m->vHEdges.at(i)->tete->voisin.at(j)->id;
                iHE2Tete = _m->vHEdges.at(i)->tete->voisin.at(j)->tete->id;
                iHE2Queue = _m->vHEdges.at(i)->tete->voisin.at(j)->queue->id;
                if ((iHETete == iHE2Queue)&&(iHEQueue == iHE2Tete))
                {
                    bTest = true;
                    _m->vHEdges.at(i)->paire = _m->vHEdges.at(iHE2ID);
                    _m->vHEdges.at(iHE2ID)->paire = _m->vHEdges.at(i);
                    //On remplit les arètes paires de chaque face
                    for (unsigned int k = 0; k < iFsize;k++)
                    {
                        iFHE = _m->vFaces.at(k)->he->id;
                        if (iFHE == iHEID)
                            _m->vFaces.at(k)->he->paire = _m->vHEdges.at(iHE2ID);
                        else if (iFHE == iHE2ID)
                            _m->vFaces.at(k)->he->paire = _m->vHEdges.at(i);
                    }
                }
            }
            if (bTest == false)
            {
                iVoisinSize = _m->vHEdges.at(i)->queue->voisin.size();
                for (unsigned int j = 0; j < iVoisinSize; j++)
                {
                    iHE2ID = _m->vHEdges.at(i)->queue->voisin.at(j)->id;
                    iHE2Tete = _m->vHEdges.at(i)->queue->voisin.at(j)->tete->id;
                    iHE2Queue = _m->vHEdges.at(i)->queue->voisin.at(j)->queue->id;
                    if ((iHETete == iHE2Queue)&&(iHEQueue == iHE2Tete))
                    {
                        bTest = true;
                        _m->vHEdges.at(i)->paire = _m->vHEdges.at(iHE2ID);
                        _m->vHEdges.at(iHE2ID)->paire = _m->vHEdges.at(i);
                        //On remplit les arètes paires de chaque face
                        for (unsigned int k = 0; k < iFsize;k++)
                        {
                            iFHE = _m->vFaces.at(k)->he->id;
                            if (iFHE == iHEID)
                                _m->vFaces.at(k)->he->paire = _m->vHEdges.at(iHE2ID);
                            else if (iFHE == iHE2ID)
                                _m->vFaces.at(k)->he->paire = _m->vHEdges.at(i);
                        }
                    }
                }
            }
            bTest = false;
        }
    }
    for (unsigned int i = 0; i < iHEsize; i++)
    {
        if(_m->vHEdges.at(i)->paire == NULL)
        {
            he->id = _m->vHEdges.size();
            he->f = NULL;
            he->nouveau = NULL;
            he->paire = _m->vHEdges.at(i);
            he->precedente = NULL;
            he->queue = _m->vHEdges.at(i)->tete;
            he->tete = _m->vHEdges.at(i)->queue;
            he->suivante = NULL;
            _m->vHEdges.push_back(he);
            iHEsize = _m->vHEdges.size();
            _m->vHEdges.at(i)->paire = _m->vHEdges.at(iHEsize-1);
            _m->nbEdges++;
        }
    }
}
void MaFenetre::VertNormal (Mesh * _m)
{
    unsigned int iVsize = _m->vVerts.size();//Taille du vector Vert
    unsigned int iVoisinSize;//Taille du vector voisin
    unsigned int iVertId;
    unsigned int iVertTete;
    unsigned int iVertQueue;
    double v[3],v1[3], v2[3];
    double dMoy [3];
    double dNorme;

    dMoy[0]=0;
    dMoy[1]=0;
    dMoy[2]=0;

    for (unsigned int i = 0; i < iVsize; i++)
    {
        iVertId = _m->vVerts.at(i)->id;
        iVoisinSize = _m->vVerts.at(i)->voisin.size();
        for (unsigned int j = 0; j < iVoisinSize; j++)
        {
            iVertTete = _m->vVerts.at(i)->voisin.at(j)->tete->id;
            iVertQueue = _m->vVerts.at(i)->voisin.at(j)->queue->id;
            {
                //Vecteur entre iVertId et iVertTete
                v1[0]=_m->vVerts.at(iVertId)->pos[0]-_m->vVerts.at(iVertTete)->pos[0];
                v1[1]=_m->vVerts.at(iVertId)->pos[1]-_m->vVerts.at(iVertTete)->pos[1];
                v1[2]=_m->vVerts.at(iVertId)->pos[2]-_m->vVerts.at(iVertTete)->pos[2];
                //Vecteur entre iVertId et iVertQueue
                v2[0]=_m->vVerts.at(iVertId)->pos[0]-_m->vVerts.at(iVertQueue)->pos[0];
                v2[1]=_m->vVerts.at(iVertId)->pos[1]-_m->vVerts.at(iVertQueue)->pos[1];
                v2[2]=_m->vVerts.at(iVertId)->pos[2]-_m->vVerts.at(iVertQueue)->pos[2];
                //Produit Vectoriel entre V1 et V2
                v[0]=v1[1]*v2[2]- v1[2]*v2[1];// Y1*Z2-Z1*Y2
                v[1]=v1[2]*v2[0]-v1[0]*v2[2];//Z1*X2-X1*Z2
                v[2]=v1[0]*v2[1]-v1[1]*v2[0];//X1*Y2-Y1*X2

                //Normalisation du vector
                dNorme = sqrt(pow(v[0], 2) + pow(v[1], 2) + pow(v[2], 2));

                v[0]=-v[0]/dNorme;
                v[1]=-v[1]/dNorme;
                v[2]=-v[2]/dNorme;
            }
            dMoy[0]+=v[0];
            dMoy[1]+=v[1];
            dMoy[2]+=v[2];
        }
        iVoisinSize = _m->vVerts.at(i)->normal[0] = dMoy[0] / iVoisinSize;
        iVoisinSize = _m->vVerts.at(i)->normal[1] = dMoy[1] / iVoisinSize;
        iVoisinSize = _m->vVerts.at(i)->normal[2] = dMoy[2] / iVoisinSize;
    }
}
int MaFenetre::MeshType (Mesh * _m)//Définit le Type du Maillage, 0 : Quelconque, 1 : Triangulaire, 2 : Carré
{
    int type = 0;
    //Face
    unsigned int FVertID;
    unsigned int HEVertID;
    unsigned int HEID;
    unsigned int iCpt = 0;
    unsigned int iMax = 0;
    unsigned int iMin = 100;
    for (unsigned int k = 0; k < 6; k++)
    {
        FVertID = _m->vFaces.at(k)->he->queue->id;

        HEID = _m->vFaces.at(k)->he->suivante->id;
        HEVertID = _m->vHEdges.at(HEID)->tete->id;

        while (HEVertID != FVertID)
        {
            iCpt++;
            HEID = _m->vHEdges.at(HEID)->suivante->id;
            HEVertID = _m->vHEdges.at(HEID)->tete->id;
        }
        if (iCpt < iMin)
            iMin = iCpt;
        else if (iCpt > iMax)
            iMax = iCpt;
        iCpt=0;

    }
    std::cout<<"iMax ("<<iMax<<") iMin ("<<iMin<<")"<<std::endl;
    if ((iMin == 1)&&(iMax == 1))
        type = 1;
    else if ((iMin == 2)&&(iMax == 2))
        type = 1;
    return type;
}
