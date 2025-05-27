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
#define MAX_LINE_LEN 1024
#define MAX_FILES     100

void prompt_name(const char* which, char* buf, size_t sz) {
    printf("Введите имя %s: ", which);
    if (!fgets(buf, (int)sz, stdin)) {
        fprintf(stderr, "Ошибка ввода\n");
        exit(1);
    }
    buf[strcspn(buf, "\r\n")] = '\0';
}

unsigned long prompt_ulong(const char* msg) {
    char* locale = setlocale(LC_ALL, "");
    char buf[MAX_LINE_LEN];
    printf("%s", msg);
    if (!fgets(buf, sizeof(buf), stdin)) {
        fprintf(stderr, "Ошибка ввода\n");
        exit(1);
    }
    buf[strcspn(buf, "\r\n")] = '\0';
    char* end;
    unsigned long v = strtoul(buf, &end, 10);
    if (end == buf || *end != '\0') {
        fprintf(stderr, "Неверное число: %s\n", buf);
        exit(1);
    }
    return v;
}

int prompt_int(const char* msg) {
    char buf[MAX_LINE_LEN];
    printf("%s", msg);
    if (!fgets(buf, sizeof(buf), stdin)) {
        fprintf(stderr, "Ошибка ввода\n");
        exit(1);
    }
    return atoi(buf);
}

int main(int argc, char* argv[]) {
    unsigned long N;
    char fres[MAX_LINE_LEN];
    int  file_count;
    char files[MAX_FILES][MAX_LINE_LEN];

    if (argc >= 4) {
        // взяли из argv
        N = strtoul(argv[1], NULL, 10);
        strcpy(fres, argv[2]);
        file_count = argc - 3;
        if (file_count > MAX_FILES) file_count = MAX_FILES;
        for (int i = 0; i < file_count; i++) {
            strcpy(files[i], argv[3 + i]);
        }
    }
    else {
        // спрашиваем у пользователя
        N = prompt_ulong("Введите пороговый размер N (в байтах): ");
        prompt_name("файла-результата", fres, sizeof(fres));
        file_count = prompt_int("Сколько файлов проанализировать? ");
        if (file_count < 1 || file_count > MAX_FILES) {
            fprintf(stderr, "Неверное количество файлов\n");
            return 1;
        }
        for (int i = 0; i < file_count; i++) {
            char which[MAX_LINE_LEN];
            snprintf(which, sizeof(which), "%d-го входного файла", i + 1);
            prompt_name(which, files[i], sizeof(files[i]));
        }
    }

    // Открываем файл-результат для записи
    FILE* fres_f = NULL;
    if (fopen_s(&fres_f, fres, "wb") != 0 || !fres_f) {
        perror("Ошибка открытия файла-результата");
        return 2;
    }

    // Обрабатываем каждый входной файл
    for (int i = 0; i < file_count; i++) {
        const char* fname = files[i];
        FILE* fin = NULL;
        if (fopen_s(&fin, fname, "rb") != 0 || !fin) {
            fprintf(stderr, "Не удалось открыть \"%s\" — пропускаем\n", fname);
            continue;
        }

        // Узнаём размер
        if (fseek(fin, 0, SEEK_END) != 0) {
            perror("fseek");
            fclose(fin);
            continue;
        }
        long size = ftell(fin);
        rewind(fin);

        if (size < 0) {
            perror("ftell");
            fclose(fin);
            continue;
        }

        if ((unsigned long)size < N) {
            // Копируем весь файл в fres
            char buffer[4096];
            size_t r;
            while ((r = fread(buffer, 1, sizeof(buffer), fin)) > 0) {
                fwrite(buffer, 1, r, fres_f);
            }
            printf("Скопирован \"%s\" (%ld байт)\n", fname, size);
        }
        else {
            printf("Пропущен  \"%s\" (%ld байт ≥ %lu)\n", fname, size, N);
        }

        fclose(fin);
    }

    fclose(fres_f);
    printf("Готово, результат в файле \"%s\"\n", fres);
    return 0;
}

#endif