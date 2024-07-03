/***************************************************************
 *
 * file: pss.h
 *
 * @Author  Nikolaos Vasilikopoulos (nvasilik@csd.uoc.gr), John Petropoulos (johnpetr@csd.uoc.gr)
 * @Version 30-11-2022
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
int a;
int b;
int m; 
int p;
int count=0;
int temporary_iId=0;
struct SubInfo *hash_array[MG];
/**
 * @brief Optional function to initialize data structures that
 *        need initialization
 *
 * @param m Size of the hash table.
 * @param p Prime number for the universal hash functions.
 *
 * @return 0 on success
 *         1 on failure
 */
int initialize(int hashTableSize, int universalHashingNumber){
	srand(time(0));
	a =(rand() % (universalHashingNumber-1)) + 1;
	b =(rand() % (universalHashingNumber-1)) + 1;
	m = hashTableSize;
	p=universalHashingNumber;
	for(int i=0;i<MG;i++){
		hash_array[i]=NULL;
	}   
    return EXIT_SUCCESS;
}

void initializeG(void){
	for(int i=0;i<MG;i++){
        G[i].gId=i;
        G[i].gr=NULL;
        G[i].gsub=NULL;
    }
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
    int temporary[MG];
    for (int r = 0; r < size_of_gids_arr - 1; r++) { 
        temporary[r]=100;
        for (int y = r + 1; y < size_of_gids_arr-1; y++) {
            if (gids_arr[r] == gids_arr[y]) {
                temporary[r]=y;
            }
        }
    }
    struct Info *temp, *prev, *new_Info;
    for(int j=0;j<size_of_gids_arr-1;j++){
		//check for gids bigger than MG-1
		if(gids_arr[j]>MG-1){
			continue;
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
		
        temp=G[gids_arr[j]].gr;
        prev=NULL;
        new_Info=(struct Info*)malloc(sizeof(struct Info));
        new_Info->iId=iId;
        new_Info->itm=iTM;
        new_Info->ilc=NULL;
        new_Info->irc=NULL;
        new_Info->ip=NULL;
        int k=0;
		while(k<size_of_gids_arr-1){          
			if(gids_arr[k]<MG && gids_arr[k]>=0){
				new_Info->igp[gids_arr[k]]=1;
				k++;
			}else{
				k++;
			}
		
		}
		for(int i=0;i<MG;i++){
			if(new_Info->igp[i]!=1){
				new_Info->igp[i]=0;
			}else{
				continue;
			}
		}
		if(temp==NULL){
			G[gids_arr[j]].gr=new_Info;
			continue;
		}else{
			while(temp!=NULL){
				if(temp->iId>=iId){
					prev = temp;
            		temp = temp->ilc;
				}else{
					prev = temp;
            		temp = temp->irc;

				}
			}
			if (prev->iId >= iId){
				prev->ilc = new_Info;
				new_Info->ip = prev;
			}else{
				prev->irc = new_Info;
				new_Info->ip = prev;
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
    struct Subscription *s,*new_s;
	//check for double sId in subscription list of each group
	for(int k=0;k<MG;k++){
        if(G[k].gsub!=NULL){
            s = G[k].gsub;
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
	int temporary[MG];
	for (int r = 0; r < size_of_gids_arr - 1; r++) { 
        temporary[r]=100;
        for (int y = r + 1; y < size_of_gids_arr-1; y++) {
            if (gids_arr[r] == gids_arr[y]) {
                temporary[r]=y;
            }
        }
    }
	//insertion in subscription list of each group
	for(int j=0;j<size_of_gids_arr-1;j++){ 
		if(gids_arr[j]>MG-1){
			continue;
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
		
		if(G[l].gsub!=NULL){
			new_s=(struct Subscription*)malloc(sizeof(struct Subscription));
			new_s->sId=sId;
			new_s->snext=G[l].gsub;
			G[l].gsub=new_s;
			
		}else{
			new_s=(struct Subscription*)malloc(sizeof(struct Subscription));
			new_s->sId=sId;
			G[l].gsub=new_s;
			new_s->snext=NULL;    
		}
	}
	//insertion in hashtable
	int index=hash_function(p,m,sId);
	insert_sId_to_HashTable(&hash_array[index],sId,sTM,gids_arr,size_of_gids_arr);
	
	return EXIT_SUCCESS;
}
/**
 * @brief Prune Information from server and forward it to client
 *
 * @param tm Information timestamp of arrival
 * @return 0 on success
 *          1 on failure
 */
int Prune(int tm){
	struct Info* tempo;
	struct Subscription *sub;
	struct SubInfo *subscriber;
	for(int i=0;i<MG;i++){
		while(1){
			temporary_iId=0;
			findSmallerTM(G[i].gr,tm);
			
			if(temporary_iId==0){
				break;
			}
			
			tempo=find_iId(G[i].gr,temporary_iId);
			if(tempo==NULL){
				break;
			}
			sub=G[i].gsub;
			while(sub!=NULL){
				
				int index=(a*(sub->sId)+b)%m;
				subscriber=find_sId(hash_array[index],sub->sId);
				
				if(subscriber!=NULL){
					subscriber->tgp[i]=incertTreeNodeInLIFO(subscriber->tgp[i],tempo,tempo->itm);	
				}
				sub=sub->snext;
			}			
			G[i].gr=delete_InfoTree_node(G[i].gr,temporary_iId);
		}
	}
    return EXIT_SUCCESS;
}
/**
 * @brief Consume Information for subscriber
 *
 * @param sId Subscriber identifier
 * @return 0 on success
 *          1 on failure
 */
int Consume(int sId){
	struct SubInfo *sub;
	struct TreeInfo *tree;
	int index=(a*sId +b)%m;
	sub=find_sId(hash_array[index],sId);
	if(sub==NULL){
		return EXIT_SUCCESS;
	}
	for(int i=0;i<MG;i++){
		tree=sub->tgp[i];
		if(tree==NULL){
			printf("GROUPID= < %d > TREELIST= < > NEWSGP = < >\n",i);
			sub->sgp[i]=tree;
		}else if(tree!=NULL && tree!=1){
			printf("GROUPID= < %d > TREELIST= < ",i);
			while(tree->tlc!=NULL){
				tree=tree->tlc;
			}
			while(tree->next!=NULL){
				printf("%d ",tree->tId);
				tree=tree->next;
			}
			sub->sgp[i]=tree;
			printf("%d > NEWSGP= < %d >\n",tree->tId,tree->tId);
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

	struct SubInfo *subscriber,*s;
	struct TreeInfo *tree;
	struct Subscription *tempo,*sub,*prev_tempo;

	int index=(a*sId +b)%m;
	subscriber=find_sId(hash_array[index],sId);
	for(int i=0;i<MG;i++){
		tree=subscriber->tgp[i];
		if(tree!=1){
			printf("GROUPID < %d > SUBLIST = <",G[i].gId);
			tempo=G[i].gsub;
            prev_tempo=G[i].gsub;
			while(tempo!=NULL){
				if(tempo->sId==sId){
					if(tempo==G[i].gsub){
						G[i].gsub=tempo->snext;
						break;
					}else{
						prev_tempo->snext=tempo->snext;
						break;
					}
				}
				prev_tempo=tempo;
				tempo=tempo->snext;
			}
			sub=G[i].gsub;
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
	struct SubInfo *temp,*prev;
	temp=prev=hash_array[index];
	while(temp!=NULL){
		if(temp->sId==sId){
			if(temp==hash_array[index]){
				hash_array[index]=temp->snext;
			}else{
				prev->snext=temp->snext;
			}
		}
		prev=temp;
		temp=temp->snext;
	}
	printf("SUBSCRIBERLIST= < ");
	for (int i=0; i<m; i++) {
		s=hash_array[i];
		//printf("INDEX: %d , SIDS :",i);
        while(s!=NULL){
			printf(" %d ",s->sId);
			s=s->snext;
		}
		
    }
	printf(">\n");

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
	// will print GROUPID < > INFOLIST < > SUBLIST < >
	for(int i=0;i<MG;i++){
		printf("GROUPID = < %d > INFOLIST = < ",i);
		info=G[i].gr;
		if(info!=NULL){
			inorder(info);
		}
		printf("> SUBLIST = < ");
		sub=G[i].gsub;
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
	//will print subscriber list and grouplists of each subscriber
	printf("SUBSCRIBERLIST= < ");
	for (int i=0; i<m; i++) {
		s=hash_array[i];
        while(s!=NULL){
			printf(" %d ",s->sId);
			s=s->snext;
		}
    }
	printf(">\n");
	for (int i=0; i<m; i++) {
		cur=hash_array[i];
        while(cur!=NULL){
			printf("SUBSCRIBERID =< %d > GROUPLIST= < ",cur->sId);
			for(int j=0;j<MG;j++){
				if(cur->tgp[j]!=1){
					printf("%d ",j);
				}
			}
			printf(">\n");
			cur=cur->snext;
		}
		
    }
    return EXIT_SUCCESS;
}

void inorder(struct Info* root)
{
	if (root != NULL) {
        inorder(root->ilc);
        printf("%d ", root->iId);
        inorder(root->irc);
    }else{
		return;
	}
}
void print_Insert(int* gids_arr, int num_of_gids){
	for(int i=0;i<num_of_gids-1;i++){
		if(gids_arr[i]<MG && gids_arr[i]>=0){
			printf("GROUPID= < %d > INFOLIST= < ",gids_arr[i]);
			inorder(G[gids_arr[i]].gr);
			printf(">\n");
		}else{
			continue;
		}
	}
}

int hash_function(int p, int m, int key){
	int num=a*key +b;
	return (num % m);//ebgala to %p

}

void insert_sId_to_HashTable(struct SubInfo** current_sub,int sId, int stm,int* gids_arr,int size_of_gids_arr){
	struct SubInfo *temp,*temporary;

	temp=(struct SubInfo*)malloc(sizeof(struct SubInfo));
	if(temp==NULL){
		printf("not available memory");
	}
	temp->sId=sId;
	temp->stm=stm;
	temp->snext=NULL;
	
	for(int k=0;k<MG;k++){
		temp->tgp[k]=1;
		temp->sgp[k]=1;
	}
	for(int i=0;i<size_of_gids_arr-1;i++){
		if(gids_arr[i]>MG-1 || gids_arr[i]<0){
			continue;
		}else{			
			temp->tgp[gids_arr[i]]=NULL;
			temp->sgp[gids_arr[i]]=NULL;
		}
	}
	temporary=*current_sub;
	if(temporary==NULL || temporary->sId > sId){
		if (count == p) {
            // HashArray Full
            printf("Insert Error: Hash Table is full\n");
		}
		temp->snext=temporary;
		*current_sub=temp;
	
	}else{		
        while (temporary->snext != NULL && temporary->snext->sId < temp->sId) {
            temporary = temporary->snext;
        }
        temp->snext = temporary->snext;
        temporary->snext = temp;
	}
}

void print_Subscriber_Registration(int num_of_gids, int* gids_arr){
	
	struct Subscription *sub;
	struct SubInfo *s;
	printf("SUBSCRIBERLIST= < ");
	for (int i=0; i<m; i++) {
		s=hash_array[i];
		
        while(s!=NULL){
			printf(" %d ",s->sId);
			s=s->snext;
		}
		
    }
	printf(">\n");
	for(int i=0;i<num_of_gids-1;i++){
        
		if(gids_arr[i]>=MG || gids_arr[i]<0 ){
            continue;
        }
        sub=G[gids_arr[i]].gsub;
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
	return;
}

void Print_Prune(void){
	struct Info *in;
	struct Subscription *subscription;
	struct SubInfo *s;
	struct TreeInfo *t;
	for(int i=0;i<MG;i++){
		in=G[i].gr;
		subscription=G[i].gsub;
		printf("GROUPID= < %d > INFOLIST= < ",i);
		if(in!=NULL){
			inorder(in);
			//printf("> SUBLIST= < ");
		}
		printf("> SUBLIST= < ");
		while(subscription!=NULL){
			printf("%d ",subscription->sId);
			subscription=subscription->snext;
		}
		printf(">\n");
	}
	for (int i=0; i<m; i++) {
		s=hash_array[i];
        while(s!=NULL){
			printf("\nSUBSCRIBERSID= < %d > \n",s->sId);
			for(int j=0;j<MG;j++){
				
				if(s->tgp[j]!=NULL && s->tgp[j]!=1){
					t=s->tgp[j];
					printf("GROUPLIST= < %d > TREEINFO = < ",j);
					while(t->tlc!=NULL){
						t=t->tlc;
					}
					while(t!=NULL){
						printf("%d ",t->tId);
						t=t->next;
					}
					printf(">\n");
				}
			}
			s=s->snext;
		}		
    }
}

struct TreeInfo* incertTreeNodeInLIFO(struct TreeInfo* root_tgp,struct Info* deleted_info, int ttm){
	struct TreeInfo* new_trrenode;
	new_trrenode=(struct TreeInfo*)malloc(sizeof(struct TreeInfo));
	new_trrenode->tId=deleted_info->iId;
	new_trrenode->ttm=deleted_info->itm;
	new_trrenode->tlc=NULL;
	new_trrenode->trc=NULL;
	new_trrenode->tp=NULL;
	new_trrenode->next=new_trrenode->prev=NULL;

	if(root_tgp==NULL){
		root_tgp=new_trrenode;
		return root_tgp;
	}else{
		struct TreeInfo* temp;
		temp=root_tgp;

		while(temp->tlc!=NULL){
			temp=temp->tlc;
		}
       
		while(temp->ttm <= ttm && temp->next!=NULL){
			temp=temp->next;
		}
        
		if(temp->ttm >= ttm && temp!=NULL){//i added =
			struct TreeInfo* tin;
			tin=(struct TreeInfo*)malloc(sizeof(struct TreeInfo));
            tin->tId=temp->tId;
			tin->ttm=temp->ttm;
            tin->next=tin->prev=tin->tp=tin->tlc=tin->trc=NULL;

            temp->tlc=new_trrenode;
            temp->trc=tin;

            temp->tlc->tp=temp;
            temp->trc->tp=temp;

			temp->tlc->prev=temp->prev;
            if (temp->prev!=NULL)
            {
                 temp->tlc->prev->next=temp->tlc;
            }        
			temp->trc->prev=temp->tlc;

            temp->tlc->next=temp->trc;
            temp->trc->next=temp->next;
            if (temp->next!=NULL)
            {
                 temp->trc->next->prev=temp->trc;
            }
			temp->tId=temp->tlc->tId;
            temp->ttm=temp->tlc->ttm;
            temp->next=temp->prev=NULL;
		
			return root_tgp;
			
		}else{
			struct TreeInfo* t;
			t=(struct TreeInfo*)malloc(sizeof(struct TreeInfo));

			t->tId=temp->tId;
			t->ttm=temp->ttm;
            t->next=t->prev=t->tp=t->tlc=t->trc=NULL;

            temp->trc=new_trrenode;
			temp->tlc=t;

            temp->tlc->tp=temp;
            temp->trc->tp=temp;

			temp->tlc->prev=temp->prev;
            if (temp->prev!=NULL)
            {
                 temp->tlc->prev->next=temp->tlc;
            }        
            temp->trc->prev=temp->tlc; 

            temp->trc->next=temp->next;
            if (temp->next!=NULL)
            {
                 temp->trc->next->prev=temp->trc;
            }
			temp->tlc->next=temp->trc;
			temp->next=temp->prev=NULL;

            return root_tgp;
		}
	}
}

struct Info* find_iId(struct Info *in,int iId){
	if(in==NULL){
		return NULL;
	}
	while(in!=NULL){
		if(iId<in->iId){
			in=in->ilc;
		}else if(iId>in->iId){
			in=in->irc;
		}else{
			return in;
		}
	}
	return NULL;
}

struct Info* delete_InfoTree_node(struct Info* info, int iId){
	struct Info* curr = info;
    struct Info* prev = NULL;
	if(curr->ilc==NULL && curr->irc==NULL && curr->iId==iId){
		return NULL;
		
	}
	while (curr != NULL && curr->iId != iId) {
        prev = curr;
        if (iId < curr->iId)
            curr = curr->ilc;
        else
            curr = curr->irc;
    }
 
    if (curr == NULL) {
        printf("iId not found");
    }
    
	if(curr->ilc == NULL || curr->irc == NULL){
 
        // newCurr will replace
        // the node to be deleted.
        struct Info* newCurr;
 
        if (curr->ilc == NULL){
            newCurr = curr->irc;
        }else{
            newCurr = curr->ilc;
		}
        
		if (prev == NULL){
            info=newCurr;
		}else{
			
			if (curr == prev->ilc)
				prev->ilc = newCurr;
			else
				prev->irc = newCurr;
		}
        return info;
	}else {
        
		struct Info* p = NULL;
        struct Info* temp;
 
        // Find the inorder successor
        temp = curr->irc;
        while (temp->ilc != NULL) {
            p = temp;
            temp = temp->ilc;
        }

        if (p != NULL){
            p->ilc = temp->irc;
        }else{
            curr->irc = temp->irc;
		}
        curr->iId = temp->iId;
		curr->itm=temp->itm;
		for(int i=0;i<MG;i++){
			curr->igp[i]=temp->igp[i];
		}
        return info;
    }
}

void findSmallerTM(struct Info* info, int tm){
	
	if (info == NULL) return;

 	findSmallerTM(info->ilc,tm);
 	findSmallerTM(info->irc,tm);
 	if(info->itm<=tm){
		temporary_iId=info->iId;
		
	} 

}

 struct SubInfo *find_sId(struct SubInfo *p, int sId){
	while(p!=NULL){
		if(p->sId==sId){
			return p;
		}
		p=p->snext;
	}
	return NULL;
    
 }   




