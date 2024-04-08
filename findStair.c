#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **items;
    int maxSize;
    int top;
}stack;

typedef struct node{
    stack *Stack;
    struct node *next;
}node;

typedef struct queue{
    node *rear;
    node *front;
}queue;

stack *copyStackv2(stack *originalStack, char *word, int wordLen);
stack *dequeue(queue *q);
queue* createQueue();
void freeStack(stack *s);
char *peek(stack *s);
char* pop(stack *s);
void push(char *word,stack *s);
stack *createStack(int capacity);
char **findEqualWords(char *firstWord, int *count);
void enqueue(queue *q,stack *s);
stack *findStair(char **arr, char *firstWord, char *lastWord, int countBigArr);
char **similarWordsFinder(char **arr, char *word, int* countSmallArr, int countBigArr);
stack *copyStack(stack *originalStack, char *word);
void freeQueue(queue *q);
void freeArray(char **array, int rows);
void stackPrinter(stack *s);

int main(){

    stack *DestStair = NULL;
    int flag=1;
    int count;
    char word[15],destWord[15];
    int gameExit = 1;
    while(gameExit)
    {
        flag = 1 ;
        printf("Kaynak kelime: ");
        scanf("%s",word);
        printf("Hedef kelime: ");
        scanf(" %s",destWord);
        if(strlen(word) != strlen(destWord)) 
        {
            printf("Harf sayisi uyumsuz.\n");
            flag =0;
        }
        if(flag)
        {
            char **arr = findEqualWords(word, &count); 
            int i;
            DestStair = findStair(arr,word,destWord,count);
            if(DestStair != NULL)
            {
                stackPrinter(DestStair);
                freeStack(DestStair);
            }
            else{
                printf("Uygun bir bağlantı bulunamadı.\n");
            }
            flag =0;
        }
        printf("Cikis: 0 Devam: 1\n");
        scanf("%d",&gameExit);
    }
    return 0;
}
void stackPrinter(stack *s)
{
    stack *aux = createStack(60);
    while(s->top != -1)
    {
        push(pop(s),aux);
    }
    printf("%s",pop(aux));

    while (aux->top != -1)
    {
        printf("->%s",pop(aux));
    } 
    printf("\n");
}
void freeArray(char **array, int rows) {
    // İlk olarak, her bir alt dizi (satır) serbest bırakılır
    int i=0;
    for (i = 0; i < rows; i++) {
        free(array[i]);
        array[i]=NULL;
    }
    
    // Daha sonra, satırları tutan ana dizi serbest bırakılır
    free(array);
    array=NULL;
}
stack *findStair(char **arr, char *firstWord, char *lastWord, int countBigArr) { // Ana işlemin yapıldığı fonksiyon

    int i;
    queue *q = createQueue();
    stack *s = createStack(100);
    push(firstWord,s); //Kaynak kelime stack'e pushlanır.
    enqueue(q,s); // ilk stack queue Null olmasın diye enqueue'lanır.

    while (q->front != NULL) {

        stack *s = dequeue(q);
        char *peekWord=peek(s); //okunulabilirlik açısından kontrolu bu string uzerinden saglanır. strcpy kullanımından kaçındım.
        int strLen = strlen(firstWord);
        if (strcmp(peekWord, lastWord) == 0) {
            // Hedef kelime bulundu, stack'i döndür.
            freeQueue(q);
            return s;
        } else {
            int countSmallArr = 0; //Bir sonraki fonksiyon sayesinde benzer kelimelerin sayisini saklar.
            char **similarArr = similarWordsFinder(arr, peekWord, &countSmallArr, countBigArr); 

            for (i = 0; i < countSmallArr; i++) {
                stack *newStack = copyStackv2(s, similarArr[i],strLen); // dequeue'lanan stack başka bir stacke kopyalanır ve   
                enqueue(q, newStack);                          // yeni stackimize matriste ilgili eleman pushlanır ve stack enqueue edilir.
            }
            freeStack(s); // Mevcut stack artık kullanılmadığı için serbest bırakılır.
            s=NULL;
            freeArray(similarArr, countSmallArr); // Kullanılan dizi serbest bırakılır.
            similarArr=NULL;
        }
    }
    return NULL;
}

char **similarWordsFinder(char **arr, char *word, int* countSmallArr, int countBigArr) 
{//Sozluk'ten cekilen esit harfli kelimelerin matrisinden stack'in peakindeki kelimeye benzeyen kelimelerin matrisini saklar
    int diffWordCount;
    int i,j;
    int wordCount=0;
    char **similARR=NULL;
    int len = strlen(word);

    for(i=0 ; i<countBigArr ; i++)
    {
        diffWordCount=0;
        j=0;
        while(diffWordCount <= 1 && j < len) // farklı harf sayisi 1'den fazla olanları direkt geçmek için while kullanıldı.
        {                                    // algortma, for kullanımına gore 2 kat hızlandı.
            if(arr[i][j] != word[j])
            {
                diffWordCount++;
            }
            j++;
        }
        
        if(diffWordCount == 1)
        {
            char **temp;
            temp = (char**)realloc(similARR, (wordCount + 1) * sizeof(char*));
            if (temp == NULL) {//recursive yapmaya calisirken genelde burada kod bozuluyordu.
                freeArray(similARR, wordCount);
                printf("error");
                return NULL;
            }
            similARR = temp;
            similARR[wordCount] = (char*)malloc(strlen(word) + 1);
            strcpy(similARR[wordCount],arr[i]); //benzer kelimeler similARR matrisine aktarılıyor sıra sıra.
            wordCount++;
        }
        else if(diffWordCount == 0) // Sonsuz döngüden kurtulmak için kullanılan kelimeleri eşit harfli kelimelerin
        {                           // bulunduğu array modifiye edildi.
            strcpy(arr[i],"33333333\0");
        }
    }
    *countSmallArr=wordCount; //benzer kelime matrisinde bulunan kelime sayisi.
    return similARR;
}
stack *copyStackv2(stack *originalStack, char *word, int wordLen)
{
    stack *copy=createStack(originalStack->maxSize);
    char *tmpWord=NULL;
    tmpWord=(char*)malloc(wordLen);
    int i;
    for(i=0;i<=originalStack->top;i++)
    {
        strcpy(tmpWord,originalStack->items[i]);
        push(tmpWord, copy);
    }
    
    push(word,copy);
    return copy;
}
stack *copyStack(stack *originalStack, char *word)
{
    stack *aux = createStack(originalStack->maxSize);
    stack *copy = createStack(originalStack->maxSize);
    char *word2;

    while(originalStack->top != -1) // original stack aux'a depolanır bu sayede stack ters döner.
    {
        push(pop(originalStack),aux);
    }

    while(aux->top != -1)// ters dönen aux stack'i tekrar orijinal stack ve kopya stackimiz'e pushlarız.
    {
        word2 = pop(aux);
        push(word2,copy);
        push(word2,originalStack);
    }
    push(word,copy);//matrisin ilgili elemanı kopya stack'imize pushlanır.
    freeStack(aux);
    aux=NULL;
    return copy;
}
void freeQueue(queue *q) {
    while (q->front != NULL) {
        node *temp = q->front;
        q->front = q->front->next;
        freeStack(temp->Stack);
        free(temp);
    }
    free(q);
}
stack *dequeue(queue *q)
{
    if(q->front == NULL)
    {
        printf("Queue is empty.");
        return NULL;
    }
    node *tmp = q->front;
    q->front=q->front->next;
    if(q->front == NULL)
    {
        q->rear = NULL;
    }
    stack* dequeuedStack = tmp->Stack; // Save the stack pointer
    free(tmp); // Free the memory of the removed node
    tmp=NULL;
    return dequeuedStack; // Return the saved stack pointer
}
void enqueue(queue *q,stack *s)
{
    node *newNode = (node*)malloc(sizeof(node));
    newNode->Stack=s;
    newNode->next=NULL;

    if(q->rear == NULL)
    {
        q->front=newNode;
        q->rear=newNode;
        return;
    }
    q->rear->next=newNode;
    q->rear=newNode;
    return;
}
queue* createQueue(){
    queue *q=(queue*) malloc(sizeof(queue));
    q->front=NULL;
    q->rear=NULL;
    return q;
}
void freeStack(stack *s)
{
    int i;
    for(i=0; i<= s->top;i++)
    {
        free(s->items[i]);
        s->items[i]=NULL;
    }
    free(s->items);
    s->items=NULL;
    free(s);
    s=NULL;
}
char *peek(stack *s)
{
    if(s->top == -1)
    {
        printf("Stack is empty.");
    }
    return s->items[s->top];
}
char *pop(stack *s)
{
    if(s->top == -1)
    {
        printf("Stack is empty.");
        return NULL;
    }
    char *word = s->items[s->top];

    s->top--;
    return word;
}
void push(char *word,stack *s)
{
    if(s->top == s->maxSize -1)
    {
        printf("Stack is full");
        return;
    }
    s->top++;
    s->items[s->top]=(char*)malloc((strlen(word)+1) * sizeof(char)); // +1 null karakteri için
    strcpy(s->items[s->top],word);
}
stack *createStack(int capacity)
{
    stack *s = (stack*)malloc(sizeof(stack));
    s->maxSize=capacity;
    s->items=(char**)malloc(sizeof(char*)*capacity);
    s->top=-1;
    return s;
}
char **findEqualWords(char *firstWord, int *count)//Belli uzunluktaki kelimelerin matrisini döndürür.
{
    FILE *file;
    char word[25];
    char **wordArr = NULL;
    char **tempRow;
    char *tempColumn;
    int wordLen = strlen(firstWord)+1;
    int row=0;
    file = fopen("dictionary.txt","r");
    while(fgets(word,sizeof(word),file) != NULL)
    {
        if(wordLen == strlen(word))
        {
            size_t length = strlen(word);
            if (word[length - 1] == '\n') //'\n' karakterinden kurtulmayı saglar bu sayede veriler daha guzel şekilde yazdırılır.
            {
                word[length - 1] = '\0';
            }
            tempRow=(char**)realloc(wordArr, (row+1)* sizeof(char*));
            if(tempRow == NULL)
            {
                perror("Realloc failed.");
                free(wordArr);
                return NULL;
            }
            else
            {
                wordArr=tempRow;
            }
            tempColumn=(char*)malloc(wordLen + 1);
            if(tempColumn == NULL)
            {
                perror("Realloc failed.");
                free(wordArr[row]);
                return NULL;
            }
            else
            {
                wordArr[row]=tempColumn;
            }
            strcpy(wordArr[row],word);
            row++;
        }
    }
    *count=row;
    fclose(file);
    return wordArr;
}
