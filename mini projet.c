#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("Hello world!\n");
    return 0;
}

typedef struct {
    char name[50];
    int account_number;
    char password[20];
    float balance;
} Account;

Account accounts[100];
int count = 0;

void saveAccounts() {
    FILE *f = fopen("accounts.txt", "w");
    for (int i = 0; i < count; i++)
        fprintf(f, "%s %d %s %.2f\n", accounts[i].name, accounts[i].account_number, accounts[i].password, accounts[i].balance);
    fclose(f);
}

void loadAccounts() {
    FILE *f = fopen("accounts.txt", "r");
    if (f == NULL) return;
    while (fscanf(f, "%s %d %s %f", accounts[count].name, &accounts[count].account_number, accounts[count].password, &accounts[count].balance) != EOF)
        count++;
    fclose(f);
}

void createAccount() {
    printf("Entrez le nom: ");
    scanf("%s", accounts[count].name);
    printf("Entrez un numéro de compte: ");
    scanf("%d", &accounts[count].account_number);
    printf("Entrez un mot de passe: ");
    scanf("%s", accounts[count].password);
    accounts[count].balance = 0;
    count++;
    saveAccounts();
    printf("Compte créé avec succès.\n");
}

int login() {
    int acc;
    char pass[20];
    printf("Numéro de compte: ");
    scanf("%d", &acc);
    printf("Mot de passe: ");
    scanf("%s", pass);
    for (int i = 0; i < count; i++) {
        if (accounts[i].account_number == acc && strcmp(accounts[i].password, pass) == 0)
            return i;
    }
    return -1;
}

void deposit(int i) {
    float amount;
    printf("Montant à déposer: ");
    scanf("%f", &amount);
    accounts[i].balance += amount;
    saveAccounts();
    printf("Dépôt réussi.\n");
}

void transfer(int i) {
    int destAcc;
    float amount;
    printf("Numéro du compte destinataire: ");
    scanf("%d", &destAcc);
    printf("Montant à transférer: ");
    scanf("%f", &amount);
    for (int j = 0; j < count; j++) {
        if (accounts[j].account_number == destAcc) {
            if (accounts[i].balance >= amount) {
                accounts[i].balance -= amount;
                accounts[j].balance += amount;
                saveAccounts();
                printf("Transfert réussi.\n");
            } else {
                printf("Solde insuffisant.\n");
            }
            return;
        }
    }
    printf("Compte destinataire introuvable.\n");
}

void checkBalance(int i) {
    printf("Solde actuel: %.2f\n", accounts[i].balance);
}

void deleteEmptyAccounts() {
    int i = 0;
    while (i < count) {
        if (accounts[i].balance == 0) {
            for (int j = i; j < count - 1; j++)
                accounts[j] = accounts[j + 1];
            count--;
        } else {
            i++;
        }
    }
    saveAccounts();
}

int main() {
    int choice;
    loadAccounts();
    while (1) {
        printf("\n--- Bienvenue à notre banque ---\n");
        printf("1. Créer un compte\n");
        printf("2. Se connecter\n");
        printf("3. Quitter\n");
        printf("Votre choix: ");
        scanf("%d", &choice);

        if (choice == 1) {
            createAccount();
        } else if (choice == 2) {
            int index = login();
            if (index == -1) {
                printf("Connexion échouée.\n");
            } else {
                printf("Connexion réussie.\n");
                int opt;
                do {
                    printf("\n1. Déposer de l'argent\n");
                    printf("2. Transférer de l'argent\n");
                    printf("3. Vérifier le solde\n");
                    printf("4. Supprimer les comptes vides\n");
                    printf("5. Se déconnecter\n");
                    printf("Choix: ");
                    scanf("%d", &opt);
                    switch (opt) {
                        case 1: deposit(index); break;
                        case 2: transfer(index); break;
                        case 3: checkBalance(index); break;
                        case 4: deleteEmptyAccounts(); break;
                        case 5: printf("Déconnexion...\n"); break;
                        default: printf("Choix invalide.\n");
                    }
                } while (opt != 5);
            }
        } else if (choice == 3) {
            break;
        } else {
            printf("Choix invalide.\n");
        }
    }
    return 0;
}
