/***************************************************************
 *
 * file: pss.h
 *
 * @Author  Nikolaos Vasilikopoulos (nvasilik@csd.uoc.gr)
 * @Version 20-10-2020
 *
 * @e-mail       hy240-list@csd.uoc.gr
 *
 * @brief   Implementation of the "pss.h" header file for the Public Subscribe System,
 * function definitions
 *
 *
 ***************************************************************
 */
#include <stdio.h>
#include <stdlib.h>

#include "pss.h"
struct Group G[MG];
struct SubInfo *head=NULL,*tail=NULL;
/**
 * @brief Optional function to initialize data structures that
 *        need initialization
 *
 * @return 0 on success
 *         1 on failure
 */
int initialize(void){
   struct SubInfo *t;
    for(int i=0;i<MG;i++){
        G[i].gId=i;
        G[i].gfirst=NULL;
        G[i].glast=NULL;
        G[i].ggsub=NULL;
        
    }
    
    return EXIT_SUCCESS;
}

/**
 * @brief Free resources
 *
 * @return 0 on success
 *         1 on failure
 */
int free_all(void){
    return EXIT_SUCCESS;
}

/**
 * @brief Insert info
 *
 * @param iTM Timestamp of arrival
 * @param iId Identifier of information
 * @param gids_arr Pointer to array containing the gids of the Event.
 * @param size_of_gids_arr Size of gids_arr including -1
 * @return 0 on success
 *          1 on failure
 */

int Insert_Info(int iTM,int iId,int* gids_arr,int size_of_gids_arr){
    int tempo=0;
    struct Info *i;
    
    for(int m=0;m<MG;m++){
        if(G[m].gfirst!=NULL){
            i=G[m].gfirst;
            for(int x=0;x<MG;x++){    
                if(i->iId==iId){
                    return EXIT_SUCCESS;
                }else{
                    if(i->inext!=NULL){
                        i=i->inext;
                        continue;
                    }else{
                        break;
                    }            
                } 
            }
        }else{
            continue;
        }
    }
    int temporary[MG];
    for (int r = 0; r < size_of_gids_arr- 1; r++) { // read comment by @nbro
        temporary[r]=100;
        for (int y = r + 1; y < size_of_gids_arr-1; y++) {
            if (gids_arr[r] == gids_arr[y]) {
                temporary[r]=y;
            }
        }
    }
    
    for(int j=0;j<size_of_gids_arr;j++){
        for(int v=0;v<size_of_gids_arr-1;v++){ 
            if(j==temporary[v]){
                if(gids_arr[j]==-1){
                    break;
                }else{
                    j++;
                } 
            }
        }
        if(gids_arr[j]>=MG ){
            if(gids_arr[j]==-1){
                break;
            }else{
                j++;
            }
        }else if(gids_arr[j]==-1){
            break;
        }else{        
            
            struct Info *current,*new_Info,*temp;
            for(int l=0;l<MG;l++){
                if(G[l].gId==gids_arr[j]){
                    tempo=l;
                    break;
                }
            }
                       
            if(G[tempo].gfirst==NULL){
                
                new_Info=(struct Info*)malloc(sizeof(struct Info));
                new_Info->itm=iTM;
                new_Info->iId=iId;
                int k=0;
                while(k<size_of_gids_arr-1){          
                    
                    new_Info->igp[gids_arr[k]]=1;
                    k++;
                
                }
                for(int i=0;i<MG;i++){
                    if(new_Info->igp[i]!=1){
                        new_Info->igp[i]=0;
                    }else{
                        continue;
                    }
                }
                
                G[tempo].gfirst=G[tempo].glast=new_Info;
                new_Info->inext=NULL;
                new_Info->iprev=NULL;

                continue;

            }else if(G[tempo].gfirst!=NULL){
                temp=G[tempo].gfirst;

                while(temp!=NULL && temp->inext!=NULL){  

                    if(temp==G[tempo].gfirst && temp->itm >= iTM){
                        new_Info=(struct Info*)malloc(sizeof(struct Info));
                        new_Info->itm=iTM;
                        new_Info->iId=iId;
                        int k=0;
                        while(k<size_of_gids_arr-1){
                                        
                            new_Info->igp[gids_arr[k]]=1;
                            k++;
                        
                        }
                        for(int i=0;i<MG;i++){
                            if(new_Info->igp[i]!=1){
                                new_Info->igp[i]=0;
                            }else{
                                continue;
                            }
                        }
                        new_Info->inext=temp;
                        new_Info->iprev=NULL;
                        G[tempo].gfirst=new_Info;
                        temp->iprev=new_Info; 
                        break;
                        
                    }else if((temp->itm <= iTM) && (temp->inext->itm >= iTM)){
                                      
                        new_Info=(struct Info*)malloc(sizeof(struct Info));
                        new_Info->itm=iTM;
                        new_Info->iId=iId;
                        int k=0;
                        while(k<size_of_gids_arr-1){
                                        
                            new_Info->igp[gids_arr[k]]=1;
                            k++;
                        
                        }
                        for(int i=0;i<MG;i++){
                            if(new_Info->igp[i]!=1){
                                new_Info->igp[i]=0;
                            }else{
                                continue;
                            }
                        }
                        
                        new_Info->inext=temp->inext;
                        temp->inext=new_Info;
                        new_Info->iprev=temp;
                        temp->inext->iprev=new_Info;
                        break;
                          
                    }else{
                        temp=temp->inext;
                    } 
                }
                if(temp==G[tempo].glast){              
                    if(temp==G[tempo].gfirst && temp->itm <=iTM){
                        new_Info=(struct Info*)malloc(sizeof(struct Info));
                        new_Info->itm=iTM;
                        new_Info->iId=iId;
                        int k=0;
                        while(k<size_of_gids_arr-1){          
                            new_Info->igp[gids_arr[k]]=1;
                            k++;
                        }
                        for(int i=0;i<MG;i++){
                            if(new_Info->igp[i]!=1){
                                new_Info->igp[i]=0;
                            }else{
                                continue;
                            }
                        }
                        temp->inext=new_Info;
                        new_Info->inext=NULL;
                        G[tempo].glast->inext=new_Info;
                        G[tempo].glast=new_Info;
                        new_Info->iprev=temp;
                        continue;
                    }else if(temp==G[tempo].gfirst && temp->itm>=iTM){
                        new_Info=(struct Info*)malloc(sizeof(struct Info));
                        new_Info->itm=iTM;
                        new_Info->iId=iId;
                        int k=0;
                        while(k<size_of_gids_arr-1){              
                            new_Info->igp[gids_arr[k]]=1;
                            k++;                       
                        }
                        for(int i=0;i<MG;i++){
                            if(new_Info->igp[i]!=1){
                                new_Info->igp[i]=0;
                            }else{
                                continue;
                            }
                        }
                        new_Info->inext=temp;
                        new_Info->iprev=NULL;
                        G[tempo].gfirst=new_Info;
                        temp->iprev=new_Info;                        
                        continue;
                    }else if(temp!=G[tempo].gfirst && temp->itm<=iTM){
                        new_Info=(struct Info*)malloc(sizeof(struct Info));
                        new_Info->itm=iTM;
                        new_Info->iId=iId;
                        int k=0;
                        while(k<size_of_gids_arr-1){                                        
                            new_Info->igp[gids_arr[k]]=1;
                            k++;                       
                        }
                        for(int i=0;i<MG;i++){
                            if(new_Info->igp[i]!=1){
                                new_Info->igp[i]=0;
                            }else{
                                continue;
                            }
                        }
                        temp->inext=new_Info;
                        new_Info->iprev=temp;
                        new_Info->inext=NULL;
                        G[tempo].glast->inext=new_Info;
                        G[tempo].glast=new_Info;                       
                        continue;
                    }else{
                        new_Info=(struct Info*)malloc(sizeof(struct Info));
                        new_Info->itm=iTM;
                        new_Info->iId=iId;
                        int k=0;
                        while(k<size_of_gids_arr-1){                                        
                            new_Info->igp[gids_arr[k]]=1;
                            k++;                        
                        }
                        for(int i=0;i<MG;i++){
                            if(new_Info->igp[i]!=1){
                                new_Info->igp[i]=0;
                            }else{
                                continue;
                            }
                        }
                        temp->iprev->inext=new_Info;
                        new_Info->iprev=temp->iprev;
                        new_Info->inext=temp;
                        temp->iprev=new_Info;                       
                        continue;   
                    }                       
                }
            }
        }       
    }   
    return EXIT_SUCCESS;
}
/**
 * @brief Subsriber Registration
 *
 * @param sTM Timestamp of arrival
 * @param sId Identifier of subscriber
 * @param gids_arr Pointer to array containing the gids of the Event.
 * @param size_of_gids_arr Size of gids_arr including -1
 * @return 0 on success
 *          1 on failure
 */
int Subscriber_Registration(int sTM,int sId,int* gids_arr,int size_of_gids_arr){
    struct Subscription *s,*new_s,*tempo;
    struct SubInfo *sub,*new_sub,*temp,*index=head;
    
    for(int m=0;m<MG;m++){
        if(G[m].ggsub!=NULL){
            s=G[m].ggsub;
            for(int x=0;x<MG;x++){    
                if(s->sId==sId){
                    return EXIT_SUCCESS;
                }else{
                    if(s->snext!=NULL){
                        s=s->snext;
                        continue;
                    }else{
                        break;
                    }            
                } 
            }
        }else{
            continue;
        }
    }
    sub=head; 
    while(sub!=NULL){   
        if(sub->sId==sId){
            return EXIT_SUCCESS;
        }else{
            if(sub->snext!=NULL){
                sub=sub->snext;
                continue;
            }else{
                break;
            }            
        } 
    }
    int temporary[MG];
    for (int r = 0; r < size_of_gids_arr - 1; r++) { // read comment by @nbro
        temporary[r]=100;
        for (int y = r + 1; y < size_of_gids_arr-1; y++) {
            if (gids_arr[r] == gids_arr[y]) {
                temporary[r]=y;
            }
        }
    }
    for(int j=0;j<size_of_gids_arr-1;j++){   
        
        if(gids_arr[j]>=MG ){
            if(gids_arr[j]==-1){
                break;
            }else{
                j++;
            }
        } 
       
        for(int b=0;b<size_of_gids_arr-1;b++){
            if(j==temporary[b]){
                if(gids_arr[j]!=-1){
                    j++;
                }else{
                    return EXIT_SUCCESS;
                }
            }
        }
        int l=gids_arr[j];
        //tempo=G[l].ggsub;
        if(G[l].ggsub!=NULL){
            new_s=(struct Subscription*)malloc(sizeof(struct Subscription));
            new_s->sId=sId;
            new_s->snext=G[l].ggsub;
            G[l].ggsub=new_s;
            
        }else{
            new_s=(struct Subscription*)malloc(sizeof(struct Subscription));
            new_s->sId=sId;
            G[l].ggsub=new_s;
            new_s->snext=NULL;    
        }   
    }
    
    if(head==NULL){
        new_sub=(struct SubInfo*)malloc(sizeof(struct SubInfo));
        new_sub->sId=sId;
        new_sub->stm=sTM;
        head=tail=new_sub;
        new_sub->snext=NULL;
        for(int k=0;k<MG;k++){
            new_sub->sgp[k]=1;
        }
        for(int i=0;i<size_of_gids_arr-1;i++){
            if(G[gids_arr[i]].glast==NULL){
                new_sub->sgp[gids_arr[i]]=NULL;
            }else{
                new_sub->sgp[gids_arr[i]]=G[gids_arr[i]].glast;
            }
        }
        return EXIT_SUCCESS;

    }else if(head!=NULL){
        temp=head;
        while(temp!=NULL && temp->snext!=NULL){            
            if(temp==head && temp->stm >= sTM){
                new_sub=(struct SubInfo*)malloc(sizeof(struct SubInfo));
                new_sub->sId=sId;
                new_sub->stm=sTM;                        
                new_sub->snext=temp;
                head=new_sub;
                for(int m=0;m<MG;m++){
                    new_sub->sgp[m]=1;
                }
                for(int a=0;a<size_of_gids_arr-1;a++){
                    if(G[gids_arr[a]].glast==NULL){
                        new_sub->sgp[gids_arr[a]]=NULL;
                    }else{
                        new_sub->sgp[gids_arr[a]]=G[gids_arr[a]].glast;
                    }
                }
                return EXIT_SUCCESS;
            }else if((temp->stm <= sTM) && (temp->snext->stm >= sTM)){                
                new_sub=(struct SubInfo*)malloc(sizeof(struct SubInfo));
                new_sub->sId=sId;
                new_sub->stm=sTM;
                new_sub->snext=temp->snext;
                temp->snext=new_sub;
                
                for(int n=0;n<MG;n++){
                    new_sub->sgp[n]=1;
                }
                for(int x=0;x<size_of_gids_arr-1;x++){
                    if(G[gids_arr[x]].glast==NULL){
                        new_sub->sgp[gids_arr[x]]=NULL;
                        continue;
                    }else{
                        new_sub->sgp[gids_arr[x]]=G[gids_arr[x]].glast;
                        continue;
                    }
                }
                
                return EXIT_SUCCESS;
            }else{
                index=temp;
                temp=temp->snext;
            }
        }
        if(temp==tail){              
            if(temp==head && temp->stm <= sTM){
                new_sub=(struct SubInfo*)malloc(sizeof(struct SubInfo));
                new_sub->sId=sId;
                new_sub->stm=sTM;
                temp->snext=new_sub;
                new_sub->snext=NULL;
                tail->snext=new_sub;
                tail=new_sub;
                for(int r=0;r<MG;r++){
                    new_sub->sgp[r]=1;
                }
                for(int f=0;f<size_of_gids_arr-1;f++){
                    if(G[gids_arr[f]].glast==NULL){
                        new_sub->sgp[gids_arr[f]]=NULL;
                    }else{
                        new_sub->sgp[gids_arr[f]]=G[gids_arr[f]].glast;
                    }
                }
                return EXIT_SUCCESS;
            }else if(temp==head && temp->stm>sTM){
                new_sub=(struct SubInfo*)malloc(sizeof(struct SubInfo));
                new_sub->sId=sId;
                new_sub->stm=sTM;
                new_sub->snext=temp;
                head=new_sub;
                for(int d=0;d<MG;d++){
                    new_sub->sgp[d]=1;
                }
                for(int v=0;v<size_of_gids_arr-1;v++){
                    if(G[gids_arr[v]].glast==NULL){
                        new_sub->sgp[gids_arr[v]]=NULL;
                    }else{
                        new_sub->sgp[gids_arr[v]]=G[gids_arr[v]].glast;
                    }
                }
                return EXIT_SUCCESS;
            }else if(temp!=head && temp->stm<=sTM){
                new_sub=(struct SubInfo*)malloc(sizeof(struct SubInfo));
                new_sub->sId=sId;
                new_sub->stm=sTM;
                temp->snext=new_sub;
                new_sub->snext=NULL;
                tail->snext=new_sub;
                tail=new_sub;
                for(int b=0;b<MG;b++){
                    new_sub->sgp[b]=1;
                }
                for(int t=0;t<size_of_gids_arr-1;t++){
                    if(G[gids_arr[t]].glast==NULL){
                        new_sub->sgp[gids_arr[t]]=NULL;
                    }else{
                        new_sub->sgp[gids_arr[t]]=G[gids_arr[t]].glast;
                    }
                }
                return EXIT_SUCCESS;
            }else{
                
                new_sub=(struct SubInfo*)malloc(sizeof(struct SubInfo));
                new_sub->sId=sId;
                new_sub->stm=sTM;
                index->snext=new_sub;
                new_sub->snext=temp;
                for(int p=0;p<MG;p++){
                    new_sub->sgp[p]=1;
                }
                for(int z=0;z<size_of_gids_arr-1;z++){
                    if(G[gids_arr[z]].glast==NULL){
                        new_sub->sgp[gids_arr[z]]=NULL;
                    }else{
                        new_sub->sgp[gids_arr[z]]=G[gids_arr[z]].glast;
                    }
                }
                return EXIT_SUCCESS;
            }
        }        
    }    
}
/**
 * @brief Consume Information for subscriber
 *
 * @param sId Subscriber identifier
 * @return 0 on success
 *          1 on failure
 */
int Consume(int sId){
    struct SubInfo *temp;
    struct Info *t, *tempo,*new_t;
    temp=head;
    while(temp!=NULL){
        if(temp->sId==sId){
            for(int j=0;j<MG;j++){
                if(temp->sgp[j]!=1 && temp->sgp[j]!=NULL){
                    printf("GROUPID < %d >",G[j].gId);
                    t=G[j].glast;
                    for(int i=0;i<MG;i++){
                        if(t==temp->sgp[j]){
                            printf(" INFOLIST <");
                            while(t!=NULL){
                                printf(" %d ",t->iId);
                                if(t->inext==NULL){
                                     temp->sgp[j]=G[j].glast;
                                }
                                t=t->inext;
                            }
                            printf("> ");
                            printf("NEWSGP = < %d >\n",temp->sgp[j]->iId);
                            break;
                        }else{
                            t=t->iprev;  
                        }
                        
                    }
                    continue;   
                }else if(temp->sgp[j]==NULL){
                    if(G[j].glast==NULL){
                        printf("GROUPID < %d >",G[j].gId);
                        printf(" INFOLIST < > ");
                        temp->sgp[j]=G[j].glast;
                        printf("NEWSGP = < >\n");
                        continue;
                    }else{                    
                        printf("GROUPID < %d >",G[j].gId);
                        tempo=G[j].gfirst;
                        printf(" INFOLIST <");
                        while(tempo!=NULL){
                            printf(" %d ",tempo->iId);
                            tempo=tempo->inext;
                        }
                        printf("> ");
                        temp->sgp[j]=G[j].glast;
                        printf("NEWSGP = < %d >\n",temp->sgp[j]->iId);
                        continue;
                    }
                }
            }
            break;
        }else{
            temp=temp->snext; 
        }
    }
    return EXIT_SUCCESS;
}

/**
 * @brief Delete subscriber
 *
 * @param sId Subscriber identifier
 * @return 0 on success
 *          1 on failure
 */
int Delete_Subscriber(int sId){
    struct SubInfo *current,*prev,*s;
    struct Subscription *tempo,*prev_tempo,*sub;
    current=head;
    prev=head;
    while(current!=NULL){
        if(current->sId==sId){
            if(current==head){
                for(int j=0;j<MG;j++){
                    if(current->sgp[j]!=1){
                        printf("GROUPID < %d > SUBLIST = < ",G[j].gId);
                        tempo=G[j].ggsub;
                        
                        prev_tempo=G[j].ggsub;
                        
                        while(tempo!=NULL){
                            if(tempo->sId==sId){
                                if(tempo==G[j].ggsub){
                                    G[j].ggsub=tempo->snext;
                                    break;
                                }else{
                                    prev_tempo->snext=tempo->snext;
                                    break;
                                }
                            }
                            prev_tempo=tempo;
                            tempo=tempo->snext;
                        }
                        sub=G[j].ggsub;
                        if(sub!=NULL){
                            for(int i=0;i<MG;i++){
                                if(sub->snext!=NULL){
                                    printf(" %d ,",sub->sId);
                                    sub=sub->snext;
                                }else{
                                    
                                    printf(" %d ",sub->sId);
                                    break;
                                                
                                }
                            }
                            printf(">\n");
                        }else{
                            printf(">\n");
                        }

                    }
                }
                head=current->snext;
            }else{
                for(int j=0;j<MG;j++){
                    if(current->sgp[j]!=1){
                        printf("GROUPID < %d > SUBLIST = < ",G[j].gId);
                        tempo=G[j].ggsub;
                        prev_tempo=G[j].ggsub;
                        while(tempo!=NULL){
                            if(tempo->sId==sId){
                                if(tempo==G[j].ggsub){
                                    G[j].ggsub=tempo->snext;
                                    break;
                                }else{
                                    prev_tempo->snext=tempo->snext;
                                    break;
                                }
                            }
                            prev_tempo=tempo;
                            tempo=tempo->snext;
                        }
                        sub=G[j].ggsub;
                        if(sub!=NULL){
                            for(int j=0;j<MG;j++){
                                if(sub->snext!=NULL){
                                    printf(" %d ,",sub->sId);
                                    sub=sub->snext;
                                }else{
                                    
                                    printf(" %d ",sub->sId);
                                    break;
                                                
                                }
                            }
                            printf(">\n");
                        }else{
                            printf(">\n");
                        }
                    }
                }
                prev->snext=current->snext;

                if(current==tail){
                    tail=prev;
                }
            }
            s=head;
            if(s!=NULL){
                printf("SUBSCRIBERLIST = < ");
                for(int i=0;i<MG;i++){
                    if(s->snext!=NULL){
                        printf(" %d ,",s->sId);
                        s=s->snext;
                    }else{
                        printf(" %d ",s->sId);
                        break;
                    } 
                }
                printf(">\n");
            }else{
                printf("maria");
            }
                    
            break;
        }else{
            prev=current;
            current=current->snext;
        }
    }
    
    return EXIT_SUCCESS;
}
/**
 * @brief Print Data Structures of the system
 *
 * @return 0 on success
 *          1 on failure
 */
int Print_all(void){
    struct Info *info;
    struct Subscription *sub;
    struct SubInfo *s,*cur;
    s=head;
    cur=head;
    int count=0;
    for(int i=0;i<MG;i++){
        info=G[i].gfirst;
        printf("GROUPID = < %d > , INFOLIST = <",G[i].gId);//afairesa to -1
        if(info!=NULL){
            int r=0;
            while(r<MG ){
                
                if(info->inext!=NULL){
                    printf(" %d ,",info->iId);
                    info=info->inext;
                }else{
                    
                    printf(" %d ",info->iId);
                    break;
                                
                }
                
            }
            printf("> SUBLIST = < ");
        }else{
            printf("> SUBLIST = < ");
        }
        sub=G[i].ggsub;
        if(sub!=NULL){
            for(int j=0;j<MG;j++){
                if(sub->snext!=NULL){
                    printf(" %d ,",sub->sId);
                    sub=sub->snext;
                }else{
                    
                    printf(" %d ",sub->sId);
                    break;               
                }
            }
            printf(">\n");
        }else{
            printf(">\n");
        }
    }
    if(s!=NULL){
        printf("SUBSCRIBERLIST = < ");
        for(int i=0;i<MG;i++){
            if(s->snext!=NULL){
                count++;
                printf(" %d ,",s->sId);
                s=s->snext;
            }else{
                count++;
                printf(" %d ",s->sId);
                break;
            } 
        }
        printf(">\n");
    }else{
        printf("maria");
    }
    
    while(cur!=NULL){
        printf("SUBSCRIBERID < %d > , GROUPLIST <",cur->sId);
        for(int l=0; l<MG; l++){
            if(cur->sgp[l]!=1){
                printf(" %d ", G[l].gId);
            }else{
                continue;
            }
        }
        printf(">\n");
        cur=cur->snext;
    }
    printf("NO_GROUPS = < %d > , NO_SUBSCRIBERS = < %d >\n",MG,count);
    return EXIT_SUCCESS;

}
/**
 * @brief Print data of Subscriber_Registration
*/
void Print_subscriber(int* gids_arr, int num_of_gids){
    struct SubInfo *s;
    struct Subscription *sub;
    s=head;
      
    if(s!=NULL){
        printf("SUBSCRIBERLIST = < ");
        for(int i=0;i<MG;i++){
            if(s->snext!=NULL){
                printf(" %d ,",s->sId);
                s=s->snext;
            }else{
                printf(" %d ",s->sId);
                break;
            } 
        }
        printf(">\n");
    }else{
        printf("maria");
    }
    
    for(int i=0;i<num_of_gids-1;i++){
        if(G[gids_arr[i]].gId>=MG){
            continue;
        }
        sub=G[gids_arr[i]].ggsub;
        printf("GROUPID = < %d > SUBLIST = <",G[gids_arr[i]].gId);
        if(sub!=NULL){
            for(int j=0;j<MG;j++){
                if(sub->snext!=NULL){
                    printf(" %d ,",sub->sId);
                    sub=sub->snext;
                }else{
                    
                    printf(" %d ",sub->sId);
                    break;
                                
                }
            }
            printf(">\n");
        }else{
            printf(">\n");
        }
    }
    
}
/**
 * Print data of Insert_Info
*/
void Print_Info(int size_ofGids_arr, int* gids_arr){
    int m=0;
    struct Info *i;
    while(m<size_ofGids_arr-1){
        if(gids_arr[m]>=MG){
            if(m+1<size_ofGids_arr-1){
                m++;
            }else{
                break;
            }
        }
        i=G[gids_arr[m]].gfirst;
        printf("GROUPID = < %d > , INFOLIST = <",G[gids_arr[m]].gId);//afairesa to -1
        if(i!=NULL){
            int r=0;
            while(r<MG ){
                
                if(i->inext!=NULL){
                    printf(" %d ,",i->iId);
                    i=i->inext;
                }else{
                    
                    printf(" %d ",i->iId);
                    break;
                                
                }
                
            }
            printf(">\n");
            m++;
        }else{
            printf(">\n");
            m++;
        }
    }
 }

