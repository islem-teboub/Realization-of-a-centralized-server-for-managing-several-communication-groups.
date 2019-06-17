
#include "Noeud.h"
#include <string.h>
#include <omnetpp.h>
#include <Segment_m.h>

/////////////////////////////////////
int i=0;
//int nbr_noeud=9;
int add=0;
int adress_s[253][3];//base de donnée de server ne peut contenir que 253 noeud
int adress_c[253][4];//ya au maximum 253 adress (253 noeud)
cQueue queue[253]; //construire 253 buffer (253 nbr de noeud max)
///////////////////////////////////////////////////////////////////////////////////
Define_Module (Server);


void Server::initialize()
{
    int nbr_noeud = getParentModule() -> par("x");

    for (int j = 0; j < nbr_noeud; j++)
            {

        Segment *m = new Segment();


        //cest un segment dhcp
        m->msg_dhcp =1;
        m->address[0]=10;
        m->address[1]=0;
        //choix aleatoire de x entre 0 et 2
        int x = uniform(0,2);//grouper les client aléatoirement en 3 groupe selon l'identificateur ss réseau
        m->address[2]=x;
        add++;
        m->address[3]=add;

        //remplir la table bdd dhcp
         adress_s[j][0]=j;//la premiere colonne contient l'id du client
         adress_s[j][1]=x;//la deuxieme colonne contient l'id du sous reseau
         adress_s[j][2]=add;//la 3eme colonne contient l'id hote
         send(m, "inoutport$o", j);

    }


}
void Server::handleMessage (cMessage *msg)
{
    int nbr_noeud = getParentModule() -> par("x");
    Segment *m = check_and_cast<Segment *>(msg);
    i++;
    m->num_seq = i;

    //supprimer le noeud emetteur du groupe
    adress_s[m->source][0]=NULL;
    adress_s[m->source][1]=NULL;
    adress_s[m->source][2]=NULL;

//envoi des msg recu à tout les noeud (diffusion)

    for (int j = 0; j < nbr_noeud; j++)
            {
               if((m->source!=j)&&( adress_s[j][1]==m->destination)){//la condition sert a veridier que le message ne sera pas renvoyé à lemetteur, et que que le message ne sera envoyé qu'aux membre du groupe que l'emetteur a choisit comme destination
                Segment *copy = m->dup();
                send(copy, "inoutport$o", j);
            }
            }

}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
Define_Module (Client);
void Client::initialize()
{
}
void Client::handleMessage (cMessage *msg)
{

   Segment *m = check_and_cast<Segment *>(msg);
    if(m->msg_dhcp!=1){
    queue[getIndex()].insert(m);
    }
    else{
        adress_c[getIndex()][0]=m->address[0];
        adress_c[getIndex()][1]=m->address[1];
        adress_c[getIndex()][2]=m->address[2];
        adress_c[getIndex()][3]=m->address[3];

        if (getIndex()==0){
              Segment *msg1 = new Segment();
              msg1->source=getIndex();
              msg1->destination=0;//choix du groupe de destination
              send(msg1, "inoutport$o");

              Segment *msg2 = new Segment();
              msg2->source=getIndex();
              msg2->destination=0;//choix du groupe de destination
              send(msg2, "inoutport$o");

              Segment *msg3 = new Segment();
              msg3->source=getIndex();
              msg3->destination=0;//choix du groupe de destination
              send(msg3, "inoutport$o");

              //le noeud 0 desactive sa pile protocolaire apres lenvoi des message
              adress_c[getIndex()][0]=0;
              adress_c[getIndex()][1]=0;
              adress_c[getIndex()][2]=0;
              adress_c[getIndex()][3]=0;
          }
    }
}
void Client::finish()
{

    int nbr_noeud = getParentModule() -> par("x");
    Segment vect[50][50];
    int i=0;
    for(int affiche=0;affiche<nbr_noeud;affiche++){
    if((getIndex()==affiche)&&(getIndex()!=0)){
///////////////////////////////////////////////////////////
        EV <<" Client " <<  getIndex() <<" > Mon adresse est la suivante : "<<  adress_c[getIndex()][0] <<"."<< adress_c[getIndex()][1] <<"."<<  adress_c[getIndex()][2] <<"."<<  adress_c[getIndex()][3] <<" \n";

      while(!queue[getIndex()].isEmpty()){
          Segment *m = (Segment *)queue[getIndex()].pop();
      vect[getIndex()][i]= *m;i++;
      }
      int j=1;
      int k=0;
      while(j<=3)
      {
          while(k<50){
                       if(vect[getIndex()][k].num_seq==j){ EV <<" Client " <<  getIndex() <<" > Le message ayant un numero de séquence = "<<  vect[getIndex()][k].num_seq <<" est consommé \n";j++;k=0;}else{k++;}
          }
          j++;k=0;
      }

    /////////////////////////////////////////////////////////
      }
    }
    }

