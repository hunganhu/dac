#include <stdio.h>
#include <string.h>
#define MAXLINE 256
#define Is_White(x)    ((x) == ' ' || (x) == '\t' || (x) == '\n')

#pragma argsused
int main(int argc, char* argv[])
{
 FILE *fp1, *fp2;
 char line[MAXLINE], dest[MAXLINE+2];
 int i, j;

 if (argc == 1) {
    fprintf (stderr, "quote input_file output_file");
    return(1);
 }
 if ((fp1 = fopen(argv[1], "r")) == NULL) {
     fprintf (stderr, "quote: cannot open %s", argv[1]);
     return(1);
 }
 if ((fp2 = fopen(argv[2], "w")) == NULL) {
     fprintf (stderr, "quote: cannot open %s", argv[2]);
     return(1);
 }
 while (fgets (line, MAXLINE+1, fp1)!= NULL) {
    for (i=strlen(line); line[i] <= ' ' && i > 0; i--)
       ;
    line[++i] = '\0';
    for (i = 0, j = 0; line[i] != '\0'; i++) {
       if (line[i] == '"' || line[i] == '\\')
         dest[j++] = '\\';
       else if (line[i] != '\n')
         dest[j++] = line[i];
    }
    dest[j] = '\0';
    fprintf (fp2, "\"%s\"\n", dest);
 }
 fclose (fp1);
 fclose (fp2);
 return (0);

}