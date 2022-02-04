# Music-Box-STM32
Un projet micro controlleur en 3ème année de IESE à Polytech Grenoble.


## Comment Compiler le Projet?

### Prérequis

• Carte STM32 avec carte fille

• La VM des TPs de micro controlleur ou les libraries pour linux

### Instructions

• Brancher une STM32 avec la carte fille à l'ordinateur.

• Taper la commande "make load" depuis le dossier source.

### En cas de problème avec la STM32

• Appuyer sur le bouton reset, puis tapez la commande "make load", puis lachez le bouton reset.

## Utilisation de la carte

• La carte necessite une alimentation de 5V via USB, ou entre 5 et 15V par la patte Vin.

• Le bouton utilisateur permet de lancer la première musique, puis de faire un cycle sur au total 4 musiques:

    ♫ Au Clair de la Lune
  
    ♫ La Berceuse de Brahm
  
    ♫ Jojo Opening
  
    ♫ 
  
• Le carré de leds indique la musique en cours.

• Passé la 4ème musique, la boite s'arrête de jouer jusqu'à que l'utilisateur rappuie sur le bouto
