Cette archive fournit 8 fichiers nécessaires au bon fonctionnement du compilateur, ainsi que 10 fichiers contenant des tests du fonctionnement : 
    - asa.c
    - asa.h
    - lexer.lex
    - parser.y
    - ts.c
    - ts.h
    - algo_init.algo
    - makefile

Les 6 premiers fichiers (asa.c, asa.h, lexer.lex, parser.y, ts.c, ts.h) sont les fichiers contenants les codes du compilateur.
Le fichier "algo_init.algo" est le fichier d'entrée du compilateur, il contient l'algorithme dans le langage algorithmique décrit plus bas.
Enfin le fichier "makefile" est le fichier permettant de compiler et créer le fichier éxecutable pour fabriquer le compilateur.

Pour créer le fichier éxécutable, il faut éxécuter la commande "make" sur une invite de commande (fenêtre permettant d'éxécuter des commandes sur un ordinateur) dans le dossier contenant les fichiers du compilateurs.
La commande make éxécutera le fichier "makefile" qui a pour commandes : 
- L'effacement des fichiers créés lors de l'éxécution du makefile (commande : rm -rf *~ *.o),
- La compilation de tous les fichiers afin de fournir un fichier éxécutable (commandes : 
                                                                                        - $(CC) $(CFLAGS) -o $@ $(OBJ) -L"/usr/lib" -lfl
                                                                                        - bison -o $@ -d $<
                                                                                        - flex -o  $@ $^
                                                                            ),
- l'éxécution de ce fichier (commande : ./arc algo_init.algo > algo.ram) qui prend en paramètre le fichier "algo_init.algo", et fournit un fichier "algo.ram" contenant les instructions dans le langage de la machine RAM.

Pour ce compilateur nous utilisons les librairies Flex et Bison, 2 librairies permettant de simplifier la lecture et l'analyse du fichier donnée en entrée.

Plusieurs tests sont donnés dans cette archive, donnant un exemple des algorithmes pouvant être compilés et traduit en langage de la machine RAM.
Pour chaque algorithme fourni, un fichier *.ram est également fourni afin de donner le code en machine RAM que le compilateur génère.
Ils ont pour noms : 
    - Test1.algo et Test1.ram
    - Test2.algo et Test2.ram
    - Test3.algo et Test3.ram
    - Test4.algo et Test4.ram
    - Test5.algo et Test5.ram

Ce compilateur fonctionne de manière a traduire tout algorithme de la forme : 
    ALGO nom_algorithme
    # lignes_commentaires
    # ...
    ENTREE var1 var2 ...
    DEBUT
        VAR var3
        VAR var4
        ...
        Instruction
        Instruction
        ...
    FIN

Avec : 
-Les Instructions écrite en majuscules (ALGO, ENTREE, DEBUT, FIN, VAR, SI, SINON, ALORS, FSI, TQ, TANT QUE, FAIRE, FTQ, AFFICHER) sont des mots réservés, ne pas les utiliser comme noms de variables.
-nom_algorithme : le nom de votre algorithme (Ligne facultative).
-# lignes_commentaires : lignes de commentaires non prises en compte par le compilateur (Lignes facultatives).
-var1 var2 ... : nom de vos variables.
-Instruction : ligne d'instruction terminée par un ";", Autant d'instructions que vous voulez peuvent se suivre avant le mot réservé FIN.

Ce compilateur prend en compte et traite:
-les instructions arithmétiques classiques,
-les instructions contenants des noms de variables,
-les instructions SI expression ALORS instructions SINON instructions FSI (Le SINON est facultatif),
-les instructions TQ/TANT QUE expression FAIRE instructions FTQ,
-les entrées (ENTREE, lecture de valeurs sur la borne d'entrée)/sorties (AFFICHER, affiche sur la borne de sortie la valeur correspondante)standards,
-la déclaration de variables dans le programme (VAR varx, permet de déclarer une variable et luis allouer l'espace nécessaire).

Ce Compilateur ne prend pas en compte ni ne traite :
-les tableaux (création, utilisation des tableaux),
-les procédures/fonctions (création et utilisation de fonction),
-une suite de comparaison (expression ET/OU expression),
-les comparaisons "inférieur ou égal" et "supérieur ou égal" (<=,>=)

Fonctionnement du compilateur : 

-Ce compilateur lit le fichier donné en entrée ligne par ligne, reconnaît les caractères qu'il peut reconnaître grâce au fichier "lexer.lex".
-Ce fichier est en communication avec le fichier "parser.y" qui va analyser les caractères renvoyés par "lexer.lex" et générer un arbre syntaxique (modéilsé sous forme de structures chaînées).
-Ensuite l'arbre généré passe dans un générateur de code qui parcours l'arbre syntaxique et génère les lignes de codes correspondant aux noeuds et feuilles de l'arbre.
-Enfin tout ce code généré est envoyé vers un fichier "algo.ram" qui permet ensuite d'éxécuter le code sur la machine ram (site du simulateur de la machine RAM : http://zanotti.univ-tln.fr/RAM/ram.php)

La grammaire choisie est ainsi car elle semble être la plus permissive dans le cadre des algo respectant la syntaxe.
Ce programme est modulaire afin de faciliter la lecture du code, mais également avoir plus de facilitée pour debugger le programme.
Ce programme contient également beaucoup de structures de données afin de simplifier la lecture et la compréhension du programme mais également pour retrouver plus facilement une structure afin de corriger un problème sans forcémment impacter les autres structures de données.