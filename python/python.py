from collections import defaultdict, Counter
# Lecture du fichier et stockage des données
logs = []
with open("network_log.txt","r",encoding="utf-8") as f:
    for ligne in f:
        ligne = ligne.strip()
        if ligne:
            date, heure, ip_source, port, protocole, statut = ligne.split(";")
            entree = {
                "date": date,
                "heure": heure,
                "ip_source":ip_source,
                "port":int(port),
                "protocole":protocole,
                "statut": statut
            }
        logs.append(entree)


# Nombre total de connexions
def Nombre_connexion():
    i = 0
    for log in logs:
        i = i +1
    return i
print("Nombre total de connexions :",Nombre_connexion())

# Nombre total de succès
def Nombre_Succes():
    i = 0
    for log in logs:
        if log["statut"]=="SUCCES":
            i = i + 1
    return i
print("Nombre total de succès :",Nombre_Succes())

# Nombre total d’échecs
def Nombre_echec():
    i = 0
    for log in logs:
        if log["statut"]=="ECHEC":
            i = i + 1
    return i

print("Nombre total d’échecs:", Nombre_echec())

# Port le plus utilisé
def Port_freq():
    port_liste = [p["port"] for p in logs]
    port_frequent = max(set(port_liste),key=port_liste.count)
    return port_frequent
print("Port le plus utilisé : Le port ",Port_freq())

# Adresse IP la plus active.
def ip_active():
    ip_liste = [ip["ip_source"] for ip in logs]
    ip_frequent = max(set(ip_liste),key=ip_liste.count)
    return ip_frequent
print("Adresse IP la plus active :", ip_active())

# Suspecte
def ip_supecte():
    ip_port_echecs = defaultdict(lambda: defaultdict(int))
    for l in logs: 
        if l["statut"] == "ECHEC": 
            ip_port_echecs[l["ip_source"]][l["port"]] += 1
    ip_suspectes = [] 
    for ip, ports in ip_port_echecs.items():
        for port, nb in ports.items(): 
            if nb > 5: 
                ip_suspectes.append((ip, port, nb))

    if ip_suspectes:
        for ip, port, nb in ip_suspectes:
            print(f"- {ip} sur le port {port} ({nb} échecs)")
        return ip_suspectes
    else: 
        print("- Aucune IP suspecte détectée")

ip_suspectes=ip_supecte()

# Top 3 des ports les plus utilisés
def trois_port():
    ports = Counter([l["port"] for l in logs]) 
    top_ports = ports.most_common(3)
    print("Top 3 des ports les plus utilisés :") 
    for port, nb in top_ports:
        print(f"- Port {port} : {nb} utilisations")
    return top_ports
top_ports=trois_port()


#rapport_analyse.txt
with open("rapport_analyse.txt", "w", encoding="utf-8") as f:
    f.write("--------------------------------------------------\n")
    f.write("        === Rapport d'analyse réseau ===\n")
    f.write("--------------------------------------------------\n")
    f.write("Résumé des statistiques :\n")
    f.write(f"Nombre total de connexions :{Nombre_connexion()} \n")
    f.write(f"Nombre total de succès : {Nombre_Succes()}\n")
    f.write(f"Nombre total d'échecs: : {Nombre_echec()}\n")
    f.write(f"Port le plus utilisé : Le port {Port_freq()}\n")
    
    for port, nb in top_ports:
        f.write(f"- Port {port} : {nb} utilisations \n")
    
    if ip_suspectes:
        for ip, port, nb in ip_suspectes:
            f.write(f"- {ip} sur le port {port} ({nb} échecs) \n")
    else: 
        f.write("- Aucune IP suspecte détectée")

