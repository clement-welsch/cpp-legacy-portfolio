# Restauration — travaux non commencés

Aucune correction de compilation, consolidation d’algorithmes, migration Qt ou modification des sources importées.

1. Examiner les README et variantes avant de choisir une base par projet.
2. Localiser les dépendances manquantes et identifier les outils disponibles.
3. Intégrer ensuite les cibles CMake une par une ; préserver le comportement historique.
4. Valider chargement des assets, résultats numériques et interactions.
5. Développer le launcher seulement après disponibilité des premiers exécutables.

## Points déjà identifiés

- LODInstancing/OceanLOD référencent des fichiers Common à l’extérieur de Rework et les variables DXSDK_DIR/D3DCODER_DIR. La base Common n’a pas été reconstituée depuis un autre tutoriel. Les chemins sont volontairement inchangés.
- Qt-AffichageOBJ et Dessin utilisent des générations historiques de Qt. Les fichiers moc/ui générés sont exclus ; les fichiers .ui originaux sont conservés lorsqu’ils sont présents.
- Les TP géométriques utilisent gmtools et Makefile.global. Le support est conservé séparément ; les anciens chemins relatifs ne sont pas corrigés.
- ODE est isolé sous third_party ; l’application Dominos reste inchangée.
- Certaines fonctions restent vides : Casteljau, chargement de surfaces, réorientation des normales et certains opérateurs morphologiques. Aucune fonctionnalité absente n’est présentée comme réalisée.
- Les documents médicaux sont des travaux pédagogiques ; aucun algorithme n’a été validé pour un usage clinique.
