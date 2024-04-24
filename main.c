#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void convertc(char* s, int u);
void convertx(char* s, int u);
void convertj(char* s, int u);

int main() {
    printf("Enter file path: ");
    char s[100];
    if (scanf("%s", s) != 1) {
        printf("Error reading file path.\n");
        return 0;
    }

    char *vext = "";
    int len = strlen(s);
    if (len >= 4) {
        vext = s + len - 3;
    } else {
        printf("File path is too short.\n");
        return 0;
    }

    char *ext[] = {"csv", "xml", "json"};
    int isValid = 0;
    char* typ;
    for (int i = 0; i < 3; i++) {
        if (strcmp(vext, ext[i]) == 0) {
            isValid = 1;
            typ=ext[i];
            break;
        }
    }

    if (isValid) {
        printf("The file extension is valid.\n");
        printf("The file destination: %s \n", s);
        if(strcmp(typ,ext[0])==0){
            printf("In which format do you want to convert your %s file ?\n\t1-XML\n\t2-JSON\n",ext[0]);
            int u;
            char us[5];
            scanf("%s",us);
            u=atoi(us);
            while(u!=1 && u!=2){
                printf("Choice not recognised\n\tPut another one :\n");
                scanf("%s",us);
                u=atoi(us);
            }
            printf("Choice :%d\n",u);
            convertc(s,u);
        }else if(strcmp(typ,ext[1])==0){
            printf("In which format do you want to convert your %s file ?\n\t1-CSV\n\t2-JSON\n",ext[1]);
            int u;
            char us[5];
            scanf("%s",us);
            u=atoi(us);
            while(u!=1 && u!=2){
                printf("Choice not recognised\n\tPut another one :\n");
                scanf("%s",us);
                u=atoi(us);
            }
            printf("Choice :%d",u);
            convertx(s,u);
        }else{
            printf("In which format do you want to convert your %s file ?\n\t1-CSV\n\t2-XML\n",ext[1]);
            int u;
            char us[5];
            scanf("%s",us);
            u=atoi(us);
            while(u!=1 && u!=2){
                printf("Choice not recognised\n\tPut another one :\n");
                scanf("%s",us);
                u=atoi(us);
            }
            printf("Choice :%d",u);
            convertj(s,u);
        }
    } else {
        printf("The file extension is not valid.\n");
    }


    return 0;
}

void removeSpacesAndQuotes(char* str) {
    int i = 0, j = 0;
    while (str[i]) {
        if (str[i] != ' ' && str[i] != '"') {
            str[j++] = str[i];
        }
        i++;
    }
    str[j] = '\0';
}

void convertc(char* s, int u) {
    if(u==1){
    FILE* f = fopen(s, "r");
    FILE* fw = fopen("result.xml", "w");
    if (f == NULL || fw == NULL) {
        printf("An error occurs while opening files\n");
        if (f != NULL) fclose(f);
        if (fw != NULL) fclose(fw);
        return;
    }
    printf("File %s opened successfully for conversion to XML\n", s);

    char buffer[100] = {0};
    const char delim[] = ",";
    char* ent = strtok(fgets(buffer, 100, f), delim);
    int numColumns = 0;
    while (ent != NULL) {
        numColumns++;
        removeSpacesAndQuotes(ent);
        ent = strtok(NULL, delim);
    }

    fseek(f, 0, SEEK_SET);

    char* entt[numColumns];
    for (int i = 0; i < numColumns; i++) {
        entt[i] = malloc(100);
        if (entt[i] == NULL) {
            printf("Memory allocation failed\n");
            fclose(f);
            fclose(fw);
            return;
        }
    }

    ent = strtok(fgets(buffer, 100, f), delim);
    int p = 0;
    while (ent != NULL && p < numColumns) {
        removeSpacesAndQuotes(ent);
        strcpy(entt[p], ent);
        p++;
        ent = strtok(NULL, delim);
    }


    fprintf(fw, "<data>\n");
    while (fgets(buffer, sizeof(buffer), f) != NULL) {
        fprintf(fw, "\t<record>\n");
        ent = strtok(buffer, delim);
        p = 0;
        while (ent != NULL && p < numColumns) {
            removeSpacesAndQuotes(ent);
            fprintf(fw, "\t\t<%s>%s</%s>\n", entt[p], ent, entt[p]);
            p++;
            ent = strtok(NULL, delim);
        }
        fprintf(fw, "\t</record>\n");
    }


    fprintf(fw, "</data>\n");

    for (int i = 0; i < numColumns; i++) {
        free(entt[i]);
    }

    fclose(f);
    fclose(fw);

    printf("Conversion from CSV to XML successful.\n");
}else{
    FILE* f = fopen(s, "r");
    FILE* fw = fopen("result.json", "w");
    if (f == NULL || fw == NULL) {
        printf("An error occurs while opening files\n");
        if (f != NULL) fclose(f);
        if (fw != NULL) fclose(fw);
        return;
    }
    printf("File %s opened successfully for conversion to JSON\n", s);

    char buffer[100] = {0};
    const char delim[] = ",";
    char* ent = strtok(fgets(buffer, 100, f), delim);
    int numColumns = 0;
    while (ent != NULL) {
        numColumns++;
        ent = strtok(NULL, delim);
    }

    fseek(f, 0, SEEK_SET);

    char* entt[numColumns];
    for (int i = 0; i < numColumns; i++) {
        entt[i] = malloc(100);
        if (entt[i] == NULL) {
            printf("Memory allocation failed\n");
            fclose(f);
            fclose(fw);
            return;
        }
    }

    ent = strtok(fgets(buffer, 100, f), delim);
    int p = 0;
    while (ent != NULL && p < numColumns) {
        strcpy(entt[p], ent);
        p++;
        ent = strtok(NULL, delim);
    }


    fprintf(fw, "{\n");
    while (fgets(buffer, sizeof(buffer), f) != NULL) {
        fprintf(fw, "\t{\n");
        ent = strtok(buffer, delim);
        p = 0;
        while (ent != NULL && p < numColumns) {
            removeSpacesAndQuotes(ent);
            fprintf(fw, "\t\t%s:\"%s\",\n", entt[p], ent, entt[p]);
            p++;
            ent = strtok(NULL, delim);
        }
        fprintf(fw, "\t},\n");
    }


    fprintf(fw, "}\n");

    for (int i = 0; i < numColumns; i++) {
        free(entt[i]);
    }

    fclose(f);
    fclose(fw);

    printf("Conversion from CSV to JSON successful.\n");
}
}




void convertx(char* s,int u) {
    if(u==1){
    FILE* f = fopen(s, "r");
    FILE* fw = fopen("result.csv", "w");
    if (f == NULL || fw == NULL) {
        printf("An error occurs while opening files\n");
        if (f != NULL) fclose(f);
        if (fw != NULL) fclose(fw);
        return;
    }
    printf("File %s opened successfully for conversion to CSV\n", s);

    char line[1000];
    while (fgets(line, sizeof(line), f)) {
        char *start_tag = strchr(line, '<');
        char *end_tag;
        char tag_name[100];
        char tag_content[100];

        while (start_tag != NULL) {
            end_tag = strchr(start_tag, '>');
            if (end_tag != NULL) {
                strncpy(tag_name, start_tag + 1, end_tag - start_tag - 1);
                tag_name[end_tag - start_tag - 1] = '\0';

                char *start_content = end_tag + 1;
                char *end_content = strchr(start_content, '<');
                if (end_content != NULL) {
                    strncpy(tag_content, start_content, end_content - start_content);
                    tag_content[end_content - start_content] = '\0';

                    fprintf(fw, "%s,%s\n", tag_name, tag_content);
                }
            }
            start_tag = strchr(end_tag, '<');
        }
    }
    fclose(f);
    fclose(fw);
}else{
    FILE* f = fopen(s, "r");
    FILE* fw = fopen("result.json", "w");
    if (f == NULL || fw == NULL) {
        printf("An error occurs while opening files\n");
        if (f != NULL) fclose(f);
        if (fw != NULL) fclose(fw);
        return;
    }
    printf("File %s opened successfully for conversion to JSON\n", s);

    char line[1000];
    fprintf(fw, "{\n");
    while (fgets(line, sizeof(line), f)) {
        char *start_tag = strchr(line, '<');
        char *end_tag;
        while (start_tag != NULL) {
            end_tag = strchr(start_tag, '>');
            if (end_tag != NULL) {
                fprintf(fw, "{");
                char tag_name[100];
                char tag_content[100];
                strncpy(tag_name, start_tag + 1, end_tag - start_tag - 1);
                tag_name[end_tag - start_tag - 1] = '\0';
                char *start_content = end_tag + 1;
                char *end_content = strchr(start_content, '<');
                if (end_content != NULL) {
                    strncpy(tag_content, start_content, end_content - start_content);
                    tag_content[end_content - start_content] = '\0';

                    fprintf(fw, "\"%s\":\"%s\",\n", tag_name, tag_content);
                }
            }
            start_tag = strchr(end_tag, '<');
        }
    }
    fprintf(fw, "}");
    fclose(f);
    fclose(fw);
    }
}
void convertj(char* s, int u) {
    printf("JSON conversion not implemented yet.\n");
}
