# Projet Architecture des Systèmes d'Exploitation (n7OS)

Le but du projet est de réaliser des briques de base permettant de gérer un système à base de processeur X86. Projet réalisé dans le cadre du parcours ASR de 2ème année à l'N7.

## Fonctionnalités

- Console
- Interruptions
- Timer
- Appels systèmes (write, shutdown, fork, exit, getpid, sleep)
- Processus
- Ordonnancement FIFO avec utilisation du timer

## Usage

### Avec Docker (Mac M1)

La compilation se fait dans le container Docker.
L'exécution se fait sur la machine hôte avec qemu.

#### (1ère utilisation) Créer l'image Docker à partir du Dockerfile

```shell
docker run -v "$(pwd):/n7OS" -t i386gcc .
```

#### Lancer un shell dans le docker et compiler l'OS

```shell
docker exec -it i386gcc /bin/sh
cd /n7OS
make
```

#### Exécuter l'OS dans l'émulateur

```shell
make run
```

#### Clean

```shell
make clean
```

### Sans Docker

#### Compiler l'OS

```shell
make
```

#### Exécuter l'OS dans l'émulateur

```shell
make run
```

#### Clean

```shell
make clean
```
