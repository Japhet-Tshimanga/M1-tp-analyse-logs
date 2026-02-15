# Analyse de logs réseau

## Description
Ce programme lit un fichier `network_log.txt` contenant des informations de connexion réseau (date, heure, IP source, port, protocole, statut) et génère des statistiques utiles sur l’activité réseau.  
Il produit également un fichier `rapport_analyse.txt` qui résume les résultats de l’analyse.

## Prérequis
- Fichier `network_log.txt` présent dans le même dossier que le script

## Format du fichier d’entrée
Chaque ligne du fichier `network_log.txt` doit respecter le format suivant :
DATE;HEURE;IP_SOURCE;PORT;PROTOCOLE;STATUT

## Pour exécuter 
Ecrire la commande : py python
- Les statistiques sont affichées à l'écran.
- Un fichier créé dans le dossier ou se trouve le programme

