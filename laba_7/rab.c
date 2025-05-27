#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include <sys/stat.h>
#define TASK_5
#ifdef TASK_1

#define MAX_LINE_LEN 1024
#define MAX_LINES     10000

typedef struct {
    char* line;
    int   count;
} LineCount;

int main(void) {
    char* locale = setlocale(LC_ALL, "");
    FILE* file = NULL;
    if (fopen_s(&file, "1.txt", "r") != 0 || file == NULL) {
        perror("Не удалось открыть файл");
        return 1;
    }
    LineCount lines[MAX_LINES] = { 0 };
    int total = 0;
    char buffer[MAX_LINE_LEN];

    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\r\n")] = '\0';
        int found = 0;
        for (int i = 0; i < total; i++) {
            if (strcmp(lines[i].line, buffer) == 0) {
                lines[i].count++;
                found = 1;
                break;
            }
        }
        if (!found) {
            if (total >= MAX_LINES) {
                fprintf(stderr, "Слишком много уникальных строк\n");
                break;
            }
            lines[total].line = _strdup(buffer);
            if (!lines[total].line) {
                fprintf(stderr, "Ошибка выделения памяти\n");
                break;
            }
            lines[total].count = 1;
            total++;
        }
    }

    fclose(file);

    if (total > 0) {
        int max_i = 0;
        for (int i = 1; i < total; i++) {
            if (lines[i].count > lines[max_i].count) {
                max_i = i;
            }
        }
        printf("Наиболее часто встречающаяся строка: \"%s\"\n", lines[max_i].line);
        printf("Количество вхождений: %d\n", lines[max_i].count);
    }
    else {
        printf("Файл пуст или произошла ошибка чтения\n");
    }
    for (int i = 0; i < total; i++) {
        free(lines[i].line);
    }
    return 0;
}

#endif
#ifdef TASK_2
int main(void) {
    char* locale = setlocale(LC_ALL, "");
    FILE* fin = NULL, * fout = NULL;
    if (fopen_s(&fin, "f.txt", "r") != 0 || fin == NULL) {
        perror("Не удалось открыть входной файл f.txt");
        return 1;
    }
    if (fopen_s(&fout, "g.txt", "w") != 0 || fout == NULL) {
        perror("Не удалось открыть выходной файл g.txt");
        fclose(fin);
        return 2;
    }
    int ch;
    while ((ch = fgetc(fin)) != EOF) {
        if (ch >= 'A' && ch <= 'Z') {
            ch = ch - 'A' + 'a';
        }
        fputc(ch, fout);
    }
    fclose(fin);
    fclose(fout);
    printf("Done\n");
    return 0;
}

#endif

#ifdef TASK_3

#define MAX_LINE_LEN 1024

void prompt_name(const char* which, char* buf, size_t sz) {
    printf("Введите имя %s файла: ", which);
    if (!fgets(buf, (int)sz, stdin)) {
        fprintf(stderr, "Ошибка ввода\n");
        exit(1);
    }
    buf[strcspn(buf, "\r\n")] = '\0';
}

int main(int argc, char* argv[]) {
    char file1[MAX_LINE_LEN], file2[MAX_LINE_LEN], file3[MAX_LINE_LEN];
    char* locale = setlocale(LC_ALL, "");
    if (argc == 4) {
        strcpy(file1, argv[1]);
        strcpy(file2, argv[2]);
        strcpy(file3, argv[3]);
    }
    else {
        prompt_name("первого", file1, sizeof(file1));
        prompt_name("второго", file2, sizeof(file2));
        prompt_name("результирующего", file3, sizeof(file3));
    }

    FILE* f1, * f2, * fout;
    if (fopen_s(&f1, file1, "r") != 0 || !f1) {
        perror("Ошибка открытия первого файла");
        return 1;
    }
    if (fopen_s(&f2, file2, "r") != 0 || !f2) {
        perror("Ошибка открытия второго файла");
        fclose(f1);
        return 2;
    }
    if (fopen_s(&fout, file3, "w") != 0 || !fout) {
        perror("Ошибка открытия результирующего файла");
        fclose(f1); fclose(f2);
        return 3;
    }

    char line1[MAX_LINE_LEN], line2[MAX_LINE_LEN];
    char* p1 = NULL, * p2 = NULL;

    if (fgets(line1, sizeof(line1), f1)) {
        line1[strcspn(line1, "\r\n")] = '\0';
        p1 = line1;
    }
    if (fgets(line2, sizeof(line2), f2)) {
        line2[strcspn(line2, "\r\n")] = '\0';
        p2 = line2;
    }
    while (p1 != NULL && p2 != NULL) {
        if (strcmp(p1, p2) <= 0) {
            fprintf(fout, "%s\n", p1);
            if (fgets(line1, sizeof(line1), f1)) {
                line1[strcspn(line1, "\r\n")] = '\0';
                p1 = line1;
            }
            else {
                p1 = NULL;
            }
        }
        else {
            fprintf(fout, "%s\n", p2);
            if (fgets(line2, sizeof(line2), f2)) {
                line2[strcspn(line2, "\r\n")] = '\0';
                p2 = line2;
            }
            else {
                p2 = NULL;
            }
        }
    }
    while (p1 != NULL) {
        fprintf(fout, "%s\n", p1);
        if (fgets(line1, sizeof(line1), f1)) {
            line1[strcspn(line1, "\r\n")] = '\0';
        }
        else {
            break;
        }
    }
    while (p2 != NULL) {
        fprintf(fout, "%s\n", p2);
        if (fgets(line2, sizeof(line2), f2)) {
            line2[strcspn(line2, "\r\n")] = '\0';
        }
        else {
            break;
        }
    }
    fclose(f1);
    fclose(f2);
    fclose(fout);
    return 0;
}
#endif

#ifdef TASK_4
#define MAX_LINE_LEN 1024

void prompt_name(const char* which, char* buf, size_t sz) {
    printf("Введите имя %s файла: ", which);
    if (!fgets(buf, (int)sz, stdin)) {
        fprintf(stderr, "Ошибка ввода\n");
        exit(1);
    }
    buf[strcspn(buf, "\r\n")] = '\0';
}

int main(int argc, char* argv[]) {
    char* locale = setlocale(LC_ALL, "");
    char file1[MAX_LINE_LEN], file2[MAX_LINE_LEN];
    if (argc == 3) {
        strcpy(file1, argv[1]);
        strcpy(file2, argv[2]);
    }
    else {
        prompt_name("первого", file1, sizeof(file1));
        prompt_name("второго", file2, sizeof(file2));
    }

    FILE* f1 = NULL, * f2 = NULL;
    if (fopen_s(&f1, file1, "r") != 0 || !f1) {
        perror("Ошибка открытия первого файла");
        return 1;
    }
    if (fopen_s(&f2, file2, "r") != 0 || !f2) {
        perror("Ошибка открытия второго файла");
        fclose(f1);
        return 2;
    }

    int c1, c2;
    int line = 1, col = 1;

    while (1) {
        c1 = fgetc(f1);
        c2 = fgetc(f2);
        if (c1 == EOF && c2 == EOF) {
            printf("[0, 0] — файлы полностью совпадают\n");
            break;
        }
        if (c1 == EOF || c2 == EOF) {
            printf("[%d, %d] — один файл является началом другого\n", line + 1, 1);
            break;
        }
        if (c1 != c2) {
            printf("[%d, %d] — первое отличие\n", line, col);
            break;
        }
        if (c1 == '\n') {
            line++;
            col = 1;
        }
        else {
            col++;
        }
    }
    fclose(f1);
    fclose(f2);
    return 0;
}
#endif 

#ifdef TASK_5
int main(int argc, char* argv[]) {
    char* locale = setlocale(LC_ALL, "");
    if (argc < 4) {
        fprintf(stderr, "Используйте: %s N fres f1 f2...\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);          // Число N передается первым аргументом
    const char* result_filename = argv[2];  // Имя файла-результата

    FILE* result_file = fopen(result_filename, "wb");
    if (!result_file) {
        perror("Ошибка открытия файла-результата");
        return 1;
    }

    // Перебираем все оставшиеся аргументы (это имена файлов)
    for (int i = 3; i < argc; i++) {
        struct stat st;
        if (stat(argv[i], &st) == -1) {
            perror("Ошибка проверки размера файла");
            continue;
        }

        // Если размер файла меньше N, копируем его содержимое в файл-результат
        if (st.st_size < N) {
            FILE* current_file = fopen(argv[i], "rb");
            if (!current_file) {
                perror("Ошибка открытия текущего файла");
                continue;
            }

            unsigned char buffer[BUFSIZ];     // Буфер для чтения
            size_t bytes_read;

            // Копирование содержимого файла в файл-результат
            while ((bytes_read = fread(buffer, 1, BUFSIZ, current_file))) {
                fwrite(buffer, 1, bytes_read, result_file);
            }

            fclose(current_file);
        }
    }

    fclose(result_file);
    return 0;
}

#endif