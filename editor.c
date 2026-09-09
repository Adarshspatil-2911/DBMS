#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 4
#define MAX_LINE_LEN 256

typedef struct {
    char **lines;
    int count;
    int capacity;
} Document;

void initDocument(Document *doc) {
    doc->capacity = INITIAL_CAPACITY;
    doc->count = 0;
    doc->lines = malloc(doc->capacity * sizeof(char *));
}

void freeDocument(Document *doc) {
    for (int i = 0; i < doc->count; i++) {
        free(doc->lines[i]);
    }
    free(doc->lines);
    doc->lines = NULL;
    doc->count = 0;
    doc->capacity = 0;
}

void growIfNeeded(Document *doc) {
    if (doc->count == doc->capacity) {
        doc->capacity *= 2;
        doc->lines = realloc(doc->lines, doc->capacity * sizeof(char *));
    }
}

// Person 1 Core Functions
void displayDocument(const Document *doc) {
    if (doc->count == 0) {
        printf("(empty document)\n");
        return;
    }
    for (int i = 0; i < doc->count; i++) {
        printf("%d: %s\n", i + 1, doc->lines[i]);
    }
}

int insertLine(Document *doc, int lineNum, const char *text) {
    if (lineNum < 1 || lineNum > doc->count + 1) {
        printf("Error: Line number out of range (1 to %d)\n", doc->count + 1);
        return 0;
    }

    growIfNeeded(doc);
    int idx = lineNum - 1;

    for (int i = doc->count; i > idx; i--) {
        doc->lines[i] = doc->lines[i - 1];
    }

    doc->lines[idx] = strdup(text);
    doc->count++;
    return 1;
}

int deleteLine(Document *doc, int lineNum) {
    if (lineNum < 1 || lineNum > doc->count) {
        printf("Error: Invalid line number\n");
        return 0;
    }

    int idx = lineNum - 1;
    free(doc->lines[idx]);

    for (int i = idx; i < doc->count - 1; i++) {
        doc->lines[i] = doc->lines[i + 1];
    }

    doc->count--;
    return 1;
}

// File I/O
int saveFile(const Document *doc, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error: Could not save file %s\n", filename);
        return 0;
    }
    for (int i = 0; i < doc->count; i++) {
        fprintf(fp, "%s\n", doc->lines[i]);
    }
    fclose(fp);
    printf("Successfully saved to %s\n", filename);
    return 1;
}

int loadFile(Document *doc, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Error: Could not open file %s\n", filename);
        return 0;
    }
    freeDocument(doc);
    initDocument(doc);

    char buf[MAX_LINE_LEN];
    while (fgets(buf, sizeof(buf), fp)) {
        buf[strcspn(buf, "\n")] = '\0';
        growIfNeeded(doc);
        doc->lines[doc->count++] = strdup(buf);
    }
    fclose(fp);
    printf("Successfully loaded %s\n", filename);
    return 1;
}

// Search & Stats
void searchWord(const Document *doc, const char *word) {
    int found = 0;
    for (int i = 0; i < doc->count; i++) {
        if (strstr(doc->lines[i], word) != NULL) {
            printf("Found on line %d: %s\n", i + 1, doc->lines[i]);
            found = 1;
        }
    }
    if (!found) {
        printf("\"%s\" not found.\n", word);
    }
}

void showStats(const Document *doc) {
    int wordCount = 0;
    for (int i = 0; i < doc->count; i++) {
        char temp[MAX_LINE_LEN];
        strncpy(temp, doc->lines[i], MAX_LINE_LEN - 1);
        temp[MAX_LINE_LEN - 1] = '\0';
        char *tok = strtok(temp, " \t");
        while (tok != NULL) {
            wordCount++;
            tok = strtok(NULL, " \t");
        }
    }
    printf("Lines: %d, Words: %d\n", doc->count, wordCount);
}

int main() {
    Document doc;
    initDocument(&doc);

    char command;
    char buffer[MAX_LINE_LEN];

    while (1) {
        printf("> ");
        if (scanf(" %c", &command) != 1) break;

        if (command == 'q') {
            break;
        } else if (command == 'p') {
            displayDocument(&doc);
        } else if (command == 'i') {
            int lineNum;
            scanf("%d", &lineNum);
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = 0;
            char *text = buffer;
            if (*text == ' ') text++;
            insertLine(&doc, lineNum, text);
        } else if (command == 'd') {
            int lineNum;
            scanf("%d", &lineNum);
            deleteLine(&doc, lineNum);
        } else if (command == 's') {
            scanf("%s", buffer);
            saveFile(&doc, buffer);
        } else if (command == 'l') {
            scanf("%s", buffer);
            loadFile(&doc, buffer);
        } else if (command == 'f') {
            scanf("%s", buffer);
            searchWord(&doc, buffer);
        } else if (command == 'c') {
            showStats(&doc);
        }
    }

    freeDocument(&doc);
    return 0;
} 