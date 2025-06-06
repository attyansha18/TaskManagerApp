#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_TASKS 100

struct Task {
    char title[100];
    char description[200];
    int priority;
    int isCompleted;
};

void toLowerCase(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

void saveTasksToFile(struct Task tasks[], int numTasks) {
    FILE *fp = fopen("tasks.txt", "w");
    if (!fp) {
        printf("Error saving tasks.\n");
        return;
    }
    for (int i = 0; i < numTasks; i++) {
        fprintf(fp, "%s|%s|%d|%d\n", tasks[i].title, tasks[i].description, tasks[i].priority, tasks[i].isCompleted);
    }
    fclose(fp);
}

int loadTasksFromFile(struct Task tasks[]) {
    FILE *fp = fopen("tasks.txt", "r");
    if (!fp) {
        return 0;
    }
    int count = 0;
    while (fscanf(fp, " %[^\n]|%[^|]|%d|%d\n", tasks[count].title, tasks[count].description, &tasks[count].priority, &tasks[count].isCompleted) == 4) {
        count++;
        if (count >= MAX_TASKS) break;
    }
    fclose(fp);
    return count;
}

void exportTasksToCSV(struct Task tasks[], int numTasks) {
    FILE *fp = fopen("tasks.csv", "w");
    if (!fp) {
        printf("Error writing to CSV.\n");
        return;
    }

    fprintf(fp, "Title,Description,Priority,Status\n");
    for (int i = 0; i < numTasks; i++) {
        fprintf(fp, "\"%s\",\"%s\",%s,%s\n",
            tasks[i].title,
            tasks[i].description,
            tasks[i].priority == 1 ? "High" : tasks[i].priority == 2 ? "Medium" : "Low",
            tasks[i].isCompleted ? "Completed" : "Incomplete");
    }

    fclose(fp);
    printf("Tasks exported to tasks.csv successfully.\n");
}

void addTask(struct Task tasks[], int* numTasks) {
    if (*numTasks >= MAX_TASKS) {
        printf("Task limit reached.\n");
        return;
    }

    struct Task newTask;
    printf("Enter task title: ");
    scanf(" %[^\n]", newTask.title);
    printf("Enter task description: ");
    scanf(" %[^\n]", newTask.description);
    printf("Enter task priority (1-High, 2-Medium, 3-Low): ");
    scanf("%d", &newTask.priority);
    newTask.isCompleted = 0;

    tasks[(*numTasks)++] = newTask;
    printf("Task added.\n");
}

void displayTasks(struct Task tasks[], int numTasks, int onlyPending) {
    if (numTasks == 0) {
        printf("No tasks available.\n");
        return;
    }

    const char* priorityLabel[] = {"", "High", "Medium", "Low"};

    for (int p = 1; p <= 3; p++) {
        printf("\nPriority %d (%s):\n", p, priorityLabel[p]);
        for (int i = 0; i < numTasks; i++) {
            if (tasks[i].priority == p && (!onlyPending || !tasks[i].isCompleted)) {
                printf("Title      : %s\n", tasks[i].title);
                printf("Description: %s\n", tasks[i].description);
                printf("Status     : %s\n\n", tasks[i].isCompleted ? "Completed" : "Incomplete");
            }
        }
    }
}

void markTaskAsCompleted(struct Task tasks[], int numTasks) {
    char title[100], lowerTitle[100], compare[100];
    printf("Enter task title to mark as completed: ");
    scanf(" %[^\n]", title);
    strcpy(lowerTitle, title);
    toLowerCase(lowerTitle);

    for (int i = 0; i < numTasks; i++) {
        strcpy(compare, tasks[i].title);
        toLowerCase(compare);
        if (strcmp(compare, lowerTitle) == 0) {
            tasks[i].isCompleted = 1;
            printf("Task marked as completed.\n");
            return;
        }
    }
    printf("Task not found.\n");
}

void deleteTask(struct Task tasks[], int* numTasks) {
    char title[100], lowered[100], temp[100];
    printf("Enter the title of the task to delete: ");
    scanf(" %[^\n]", title);
    strcpy(lowered, title);
    toLowerCase(lowered);

    for (int i = 0; i < *numTasks; i++) {
        strcpy(temp, tasks[i].title);
        toLowerCase(temp);
        if (strcmp(temp, lowered) == 0) {
            for (int j = i; j < *numTasks - 1; j++) {
                tasks[j] = tasks[j + 1];
            }
            (*numTasks)--;
            printf("Task deleted.\n");
            return;
        }
    }

    printf("Task not found.\n");
}

int comparePriority(const void *a, const void *b) {
    struct Task *taskA = (struct Task*)a;
    struct Task *taskB = (struct Task*)b;
    return taskA->priority - taskB->priority;
}

void sortTasksByPriority(struct Task tasks[], int numTasks) {
    qsort(tasks, numTasks, sizeof(struct Task), comparePriority);
    printf("Tasks sorted by priority.\n");
}

int main() {
    struct Task tasks[MAX_TASKS];
    int numTasks = loadTasksFromFile(tasks);
    int choice;

    do {
        printf("\n--- Task Management Menu ---\n");
        printf("1. Add Task\n");
        printf("2. Display All Tasks\n");
        printf("3. Display Pending Tasks Only\n");
        printf("4. Mark Task as Completed\n");
        printf("5. Delete Task\n");
        printf("6. Sort Tasks by Priority\n");
        printf("7. Export Tasks to CSV\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addTask(tasks, &numTasks);
                break;
            case 2:
                displayTasks(tasks, numTasks, 0);
                break;
            case 3:
                displayTasks(tasks, numTasks, 1);
                break;
            case 4:
                markTaskAsCompleted(tasks, numTasks);
                break;
            case 5:
                deleteTask(tasks, &numTasks);
                break;
            case 6:
                sortTasksByPriority(tasks, numTasks);
                break;
            case 7:
                exportTasksToCSV(tasks, numTasks);
                break;
            case 8:
                saveTasksToFile(tasks, numTasks);
                printf("Tasks saved. Exiting.\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }

    } while (choice != 8);

    return 0;
}
