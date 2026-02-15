#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LOGS 1000
#define MAX_IP_LEN 50


// Structure pour représenter une entrée de log
typedef struct {
    char date[20];
    char heure[20];
    char ip_source[(50)];
    int port;
    char protocole[10];
    char statut[10];
} Log;

// Structure pour compter les échecs par IP et port
typedef struct {
    char ip[MAX_IP_LEN];
    int port;
    int nb_echecs;
} Compteur;

int main() {
    FILE *f = fopen("network_log.txt", "r");
    

    Log logs[MAX_LOGS];
    int n = 0;

    // Lecture du fichier ligne par ligne
    while (n < MAX_LOGS && fscanf(f, "%[^;];%[^;];%[^;];%d;%[^;];%s\n",
                                  logs[n].date,
                                  logs[n].heure,
                                  logs[n].ip_source,
                                  &logs[n].port,
                                  logs[n].protocole,
                                  logs[n].statut) == 6) {
        n++;
    }
    fclose(f);

    // Statistiques globales
    int total = n;
    int succes = 0, echec = 0;
    for (int i = 0; i < n; i++) {
        if (strcmp(logs[i].statut, "SUCCES") == 0) succes++;
        else if (strcmp(logs[i].statut, "ECHEC") == 0) echec++;
    }

    // Port le plus utilisé
    int port_count[MAX_LOGS];
    int port_values[MAX_LOGS];
    int port_unique = 0;
    for (int i = 0; i < n; i++) {
        int found = 0;
        for (int j = 0; j < port_unique; j++) {
            if (port_values[j] == logs[i].port) {
                port_count[j]++;
                found = 1;
                break;
            }
        }
        if (!found) {
            port_values[port_unique] = logs[i].port;
            port_count[port_unique] = 1;
            port_unique++;
        }
    }
    int max_port = port_values[0], max_count = port_count[0];
    for (int j = 1; j < port_unique; j++) {
        if (port_count[j] > max_count) {
            max_port = port_values[j];
            max_count = port_count[j];
        }
    }

    // IP la plus active
    char ip_values[MAX_LOGS][MAX_IP_LEN];
    int ip_count[MAX_LOGS];
    int ip_unique = 0;
    for (int i = 0; i < n; i++) {
        int found = 0;
        for (int j = 0; j < ip_unique; j++) {
            if (strcmp(ip_values[j], logs[i].ip_source) == 0) {
                ip_count[j]++;
                found = 1;
                break;
            }
        }
        if (!found) {
            strcpy(ip_values[ip_unique], logs[i].ip_source);
            ip_count[ip_unique] = 1;
            ip_unique++;
        }
    }
    char ip_active[MAX_IP_LEN];
    int max_ip_count = ip_count[0];
    strcpy(ip_active, ip_values[0]);
    for (int j = 1; j < ip_unique; j++) {
        if (ip_count[j] > max_ip_count) {
            max_ip_count = ip_count[j];
            strcpy(ip_active, ip_values[j]);
        }
    }

    // Détection des IP suspectes (plus de 5 échecs sur un même port)
    Compteur compteurs[MAX_LOGS];
    int nb_compteurs = 0;
    for (int i = 0; i < n; i++) {
        if (strcmp(logs[i].statut, "ECHEC") == 0) {
            int found = 0;
            for (int j = 0; j < nb_compteurs; j++) {
                if (strcmp(compteurs[j].ip, logs[i].ip_source) == 0 &&
                    compteurs[j].port == logs[i].port) {
                    compteurs[j].nb_echecs++;
                    found = 1;
                    break;
                }
            }
            if (!found) {
                strcpy(compteurs[nb_compteurs].ip, logs[i].ip_source);
                compteurs[nb_compteurs].port = logs[i].port;
                compteurs[nb_compteurs].nb_echecs = 1;
                nb_compteurs++;
            }
        }
    }

    // Affichage
    printf("=== Résultats de l analyse réseau ===\n\n");
    printf("Nombre total de connexions : %d\n", total);
    printf("Nombre de Succes : %d\n", succes);
    printf("Nombre d'Echecs : %d\n\n", echec);
    printf("Port le plus utilise : %d\n", max_port);
    printf("Adresse IP la plus active : %s\n\n", ip_active);

    printf("IP suspectes (plus de 5 échecs sur un meme port) :\n");
    int suspects = 0;
    for (int i = 0; i < nb_compteurs; i++) {
        if (compteurs[i].nb_echecs > 5) {
            printf("- %s sur le port %d (%d echecs)\n",
                   compteurs[i].ip, compteurs[i].port, compteurs[i].nb_echecs);
            suspects++;
        }
    }
    if (suspects == 0) {
        printf("- Aucune IP suspecte détectee\n");
    }

    // --- Génération du rapport ---
    FILE *rapport = fopen("rapport_analyse.txt", "w");
    if (!rapport) {
        printf("Erreur : impossible de créer rapport_analyse.txt\n");
        return 1;
    }
    fprintf(rapport, "--------------------------------------------------\n");
    fprintf(rapport, "        === Rapport d'analyse réseau ===\n");
    fprintf(rapport, "--------------------------------------------------\n");
    fprintf(rapport, "Résumé des statistiques :\n");
    fprintf(rapport, "Nombre total de connexions : %d\n", total);
    fprintf(rapport, "Nombre total de succès : %d\n", succes);
    fprintf(rapport, "Nombre total d'échecs : %d\n", echec);
    fprintf(rapport, "Port le plus utilisé : %d\n", max_port);
    fprintf(rapport, "Adresse IP la plus active : %s\n", ip_active);

    if (suspects > 0) {
        for (int i = 0; i < nb_compteurs; i++) {
            if (compteurs[i].nb_echecs > 5) {
                fprintf(rapport, "- %s sur le port %d (%d échecs)\n",
                        compteurs[i].ip, compteurs[i].port, compteurs[i].nb_echecs);
            }
        }
    } else {
        fprintf(rapport, "- Aucune IP suspecte détectée\n");
    }
    fclose(rapport);

    printf("\nRapport généré dans rapport_analyse.txt\n");

    return 0;
}
