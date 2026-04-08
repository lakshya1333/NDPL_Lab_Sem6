#include<stdio.h>
int main(){
    char inp[50];
    printf("enter string:");
    scanf("%s",inp);
    int j=strlen(inp)-1;
    int i=0;
    int flag=0;
    while(i<j){
        if(inp[i]!=inp[j]){
            flag=1;
            break;
        }
        i++;
        j--;
    }
    if(flag==1){
        printf("not a palindrome");
    }else{
        printf("is a palindrome");
    }
    return 0;
}
