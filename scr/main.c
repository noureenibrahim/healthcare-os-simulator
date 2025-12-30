#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <ctype.h>

pid_t patient_pid = -1;

void log_event(const char *message){
    char cmd[300];
    snprintf(cmd, sizeof(cmd), "cd .. && echo \"[%s] %s\" >> ./logs/patients.log", "$(date '+%Y-%m-%d %H:%M:%S')", message);
    system(cmd);
}

int valid_id(char *id){
    if (strlen(id) != 5 || id[0] != 'P') return 0;
    for (int i = 1; i < 5; i++)
        if (!isdigit(id[i])) return 0;
    return 1;
}

void trim_newline(char *str){
    str[strcspn(str, "\n")] = 0;
}

void create_patient(){
    char id[10], name[50], age[10], condition[100], cmd[512];

    printf("Patient ID (Pxxxx): ");
    scanf("%9s", id); getchar();

    if (!valid_id(id)) {
        printf("Invalid ID format.\n");
        return;
    }

    printf("Name: ");
    fgets(name, sizeof(name), stdin);
    trim_newline(name);

    printf("Age: ");
    scanf("%9s", age); getchar();

    printf("Condition: ");
    fgets(condition, sizeof(condition), stdin);
    trim_newline(condition);

    snprintf(cmd, sizeof(cmd), "./manage_patient.sh create %s \"%s\" %s \"%s\"", id, name, age, condition);

    system(cmd);
    log_event("New patient record created");
}

void view_patient(){
    char folder[100], cmd[256];
    printf("Patient folder (ID_Name): ");
    fgets(folder, sizeof(folder), stdin);
    trim_newline(folder);

    snprintf(cmd, sizeof(cmd), "./manage_patient.sh view \"%s\"", folder);
    system(cmd);
}

void delete_patient(){
    char folder[100], cmd[256];
    printf("Patient folder to delete: ");
    fgets(folder, sizeof(folder), stdin);
    trim_newline(folder);

    snprintf(cmd, sizeof(cmd), "./manage_patient.sh delete \"%s\"", folder);
    system(cmd);
    log_event("Patient record deleted");
}

void list_patients(){
    system("./manage_patient.sh list");
}

void start_process(){
    if(patient_pid > 0){
        printf("Patient monitoring already running\n");
        return;
    }

    patient_pid = fork();

    if(patient_pid == 0){
        setpgid(0,0);

        while(1){
            printf("Monitoring patient vitals...\n");
            sleep(1);
        }
    }
    else if(patient_pid > 0){
        printf("Patient monitoring started (PID: %d)\n", patient_pid);
        log_event("Patient monitoring process started");
    }
    else{
        perror("fork failed");
    }
}

void stop_process(){
    if(patient_pid <=0){
        printf("No patient monitoring process to stop\n");
        return;
    }

    kill(patient_pid, SIGSTOP);
    printf("patient monitoring paused\n");
    log_event("Patient monitoring process paused (SIGSTOP)");
}

void resume_process(){
    if(patient_pid <= 0){
        printf("No patient monitoring to resume\n");
        return;
    }

    kill(patient_pid, SIGCONT);
    printf("Patient monitoring resumed\n");
    log_event("Patient monitoring process resumed (SIGCONT)");
}

void kill_process(){
    if(patient_pid <= 0){
        printf("No patient monitoring process to kill\n");
        return;
    }

    kill(patient_pid, SIGTERM);
    waitpid(patient_pid, NULL, 0);
    log_event("Patient monitoring process terminated (SIGTERM)");

    printf("Patient monitoring stopped and cleaned up\n");
    patient_pid = -1;
}

void wait_patient(){
    if (patient_pid <= 0) {
        printf("No patient process to wait for\n");
        return;
    }

    int status;
    pid_t result = waitpid(patient_pid, &status, 0);

    if (result == -1) {
        perror("waitpid failed");
        return;
    }

    if (WIFEXITED(status)) {
        printf("Patient process exited normally with status %d\n", WEXITSTATUS(status));
         log_event("Patient process exited normally");
    } 
    else if (WIFSIGNALED(status)) {
        printf("Patient process terminated by signal %d\n", WTERMSIG(status));
        log_event("Patient process terminated by signal");
    }

    patient_pid = -1; 
}

int main() {
    char input[100];

    system("clear"); 
    system("chmod +x manage_patient.sh");
    system("chmod +x patient_find.sh");
    system("chmod +x memory_manager.sh");
    printf("Welcome to HealthcareOS\n");
    printf("Type 'help' for a list of commands\n");
    log_event("HealthcareOS started");

    while (1) {

        printf("\nHealthcareOS> ");

        if (fgets(input, 100, stdin) == NULL){
            break; // Handle exit on Ctrl+D
        }

        trim_newline(input);

        if (strcmp(input, "shutdown") == 0){
            printf("System shutting down...\n");
            log_event("HealthcareOS shutting down");
            if (patient_pid > 0) {
                kill_process();
            }
            break;
        } 
        else if (strcmp(input, "system_status") == 0){
            system("./system_status.sh");
        } 
        else if (strncmp(input, "patient_find", 12) == 0){
 
            char command[150];
            
            // Safe way to format the string
            // Input is "patient_find John", we want just "John" (part after space)
            char *name = input + 13; 
            
            if (strlen(name) > 0){
                sprintf(command, "./patient_find.sh \"%s\"", name);
                system(command);
            } else {
                printf("Error: Name required. Usage: patient_find <name>\n");
            }
        }
        else if(strcmp(input, "start_process") == 0){
            start_process();
        }
        else if(strcmp(input, "stop_process") == 0){
            stop_process();
        } 
        else if(strcmp(input, "resume_process") == 0){
            resume_process();
        } 
        else if(strcmp(input, "kill_process") == 0){
            kill_process();
        } 
        else if(strcmp(input, "clear") == 0){
            system("clear");
        } 
        else if(strcmp(input, "memory_map") == 0){
            system("gcc memory.c -o mem");
            system("./mem");
        }
        else if(strcmp(input, "disk_check") == 0){
            printf("--- Checking Database Storage Capacity ---\n");
            system("df -h"); // -h makes it human-readable
        }
        else if (strncmp(input, "check_deps", 10) == 0){
        char *filename = input + 11; // Skip "check_deps " to get the file path

        char command[256];
        // We construct the command: ./memory_manager.sh deps <filename>
        snprintf(command, sizeof(command), "./memory_manager.sh %s", filename);
        system(command);
        }
        else if (strcmp(input, "vitals_mem") == 0){
        printf("--- Monitoring System Vitals (RAM) ---\n");
        system("vmstat 3 2"); // Runs vmstat every 3 seconds, 2 times
        }
        else if (strcmp(input, "patient_wait") == 0){
            wait_patient();
        }
        else if (strcmp(input, "patient_create") == 0){
            create_patient();
        }
        else if (strcmp(input, "patient_view") == 0){
            view_patient();
        }
        else if (strcmp(input, "patient_delete") == 0){
            delete_patient();
        }
        else if (strcmp(input, "patient_list") == 0){
            list_patients();
        }
        else if (strcmp(input, "help") == 0){
            printf("Available Commands:\n");
            printf("patient_create : Register a new patient\n");
            printf("patient_view <ID_Name> : View patient medical record\n");
            printf("patient_delete <ID_Name> : Delete a patient record\n");
            printf("patient_list : List all registered patients\n");
            printf("patient_find <name> : Search for a patient\n");
          //  printf("system_status : Check OS health\n");
            printf("start_process : Start patient monitoring\n");
            printf("stop_process : Pause monitoring\n");
            printf("resume_process : Resume monitoring\n");
            printf("kill_process : Stop monitoring\n");
            printf("patient_wait : Wait for patient process to finish\n");
            printf("memory_map : Inspect memory segments (Text, Data, Heap, Stack)\n");
            printf("disk_check : Check hospital database storage\n");
            printf("check_deps <file> : Check process dependencies\n");
            printf("vitals_mem : Monitor system memory health\n");
            printf("clear : Clear screen\n");
            printf("shutdown : Exit the OS\n");
        }
        else{
            printf("Unknown command: %s\n", input);
        }
    }

    return 0;
}
