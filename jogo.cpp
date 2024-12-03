#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>


// Classe base Player que define os atributos e métodos comuns dos personagens

class Player {
protected:
    int hp;
    int attack;
    int speed;
    int chosenAttack;
    std::string attack1Name, attack2Name, attack3Name;

public:
    Player() : chosenAttack(0), hp(0), attack(0), speed(0) {}
    
    // Métodos públicos para acessar os atributos protegidos
    int getChosenAttack() const { return chosenAttack; }
    void setChosenAttack(int attackChoice) { chosenAttack = attackChoice; }
    

    
    std::string getAttack1Name() const { return attack1Name; }
    std::string getAttack2Name() const { return attack2Name; }
    std::string getAttack3Name() const { return attack3Name; }
    

    // Métodos virtuais puros para os ataques específicos de cada classe
    virtual void attack1(Player &opponent) = 0;
    virtual void attack2(Player &opponent) = 0;
    virtual void attack3(Player &opponent) = 0;
    virtual void receiveDamage(int damage) = 0;

    virtual ~Player() {}

    // Métodos para acessar e modificar o HP, ataque e velocidade
    int getHp() const { return hp; }
    void setHp(int newHp) { hp = newHp; }
    int getAttack() const { return attack; }
    void setAttack(int newAttack) { attack = newAttack; }
    int getSpeed() const { return speed; }
    void setSpeed(int newSpeed) { speed = newSpeed; }

    // Sobrecarga do operador de subtração
    Player& operator-(int damage) {
        this->hp -= damage;
        return *this;
    }

    // Sobrecarga do operador de atribuição
    Player& operator=(const Player& other) {
        if (this != &other) {
            this->hp = other.hp;
            this->attack = other.attack;
            this->speed = other.speed;
            this->chosenAttack = other.chosenAttack;
            this->attack1Name = other.attack1Name;
            this->attack2Name = other.attack2Name;
            this->attack3Name = other.attack3Name;
        }
        return *this;
    }
};


// Classe Warrior que herda de Player e define ataques específicos para o Guerreiro
class Warrior : public Player {
public:
    Warrior() {
        hp = 150;
        attack = 10;
        speed = 5;
        attack1Name = "Sword Attack";
        attack2Name = "Shield Bash";
        attack3Name = "Rally";
    }

// Implementação dos ataques do Guerreiro
    void attack1(Player &opponent) override {
        opponent - 10;
    }

    void attack2(Player &opponent) override {
        opponent - 5;
    }

    void attack3(Player &opponent) override {
        hp += 20;
        if (hp > 150)
            hp = 150;
    }

    void receiveDamage(int damage) override {
        hp -= damage;
    }
};

// Classe Rogue que herda de Player e define ataques específicos para o Ladrão
class Rogue : public Player {
private:
    bool stealth; // Indica se o Ladrão está em estado de furtividade

public:
    Rogue() {
        hp = 100;
        attack = 10;
        speed = 10;
        attack1Name = "Quick Attack";
        attack2Name = "Stealth";
        attack3Name = "Backstab";
        stealth = false;
    }

    // Implementação dos ataques do Ladrão
    void attack1(Player &opponent) override {
        opponent - 10;
    }

    void attack2(Player &opponent) override {
        stealth = true;
    }

    void attack3(Player &opponent) override {
        opponent - 50;
        hp -= 20;
    }

    void receiveDamage(int damage) override {
        if (stealth) {
            std::cout << "The Rogue dodged the attack!!\n"; // O Ladrão desvia se estiver em furtividade
            stealth = false; // Desativa o estado de furtividade
        } else {
            hp -= damage; // Caso contrário, o Ladrão recebe dano
        }
    }
};

class Mage : public Player {
private:
    bool shieldActive;
    
public:
    Mage() {
        hp = 80;
        attack = 10;
        speed = 7;
        shieldActive = false;
        attack1Name = "Fireball";
        attack2Name = "Frost Nova";
        attack3Name = "Arcane Shield";
    }

    void attack1(Player &opponent) override {
        opponent - 30; // Fireball
    }

    void attack2(Player &opponent) override {
        opponent.setSpeed(0); // Reduz a velocidade do oponente a 0 com Frost Nova
    }

    void attack3(Player &opponent) override {
        shieldActive = true; // Ativa o escudo Arcano
    }

    void receiveDamage(int damage) override {
        if (shieldActive) {
            damage /= 2; // O dano é reduzido pela metade se o escudo estiver ativo
            shieldActive = false;
        }
        hp -= damage;
    }
};

class Archer : public Player {
private:
    bool eagleEyeActive;

public:
    Archer() {
        hp = 100;
        attack = 15;
        speed = 9;
        eagleEyeActive = false;
        attack1Name = "Arrow Shot";
        attack2Name = "Multi-Shot";
        attack3Name = "Eagle Eye";
    }

    void attack1(Player &opponent) override {
        opponent - 20; // Arrow Shot
    }

    void attack2(Player &opponent) override {
        if (eagleEyeActive || (rand() % 2 == 0)) {
            opponent - 15; // Multi-Shot
        } else {
            std::cout << "Multi-Shot missed!\n";
        }
    }

    void attack3(Player &opponent) override {
        eagleEyeActive = true; // Ensures next attack hits
    }

    void resetEagleEye() {
        eagleEyeActive = false;
    }

    void receiveDamage(int damage) override {
        hp -= damage;
    }
};

// Função para mostrar as opções de ataque
void showAttackOptions(Player *player) {
    std::cout << "Choose your attack: 1 - " << player->getAttack1Name() 
              << ", 2 - " << player->getAttack2Name() 
              << ", 3 - " << player->getAttack3Name() << "\n";
}

// Função para executar o ataque escolhido
void executeAttack(Player *attacker, Player *opponent) {
    switch (attacker->getChosenAttack()) {
        case 1:
            attacker->attack1(*opponent);
            break;
        case 2:
            attacker->attack2(*opponent);
            break;
        case 3:
            attacker->attack3(*opponent);
            break;
        default:
            std::cerr << "Invalid attack choice.\n";
            break;
    }
}

// Função que lida com o turno do jogador
void playerTurn(Player *attacker, Player *opponent) {
    int choice;
    bool validChoice = false;
    while (!validChoice) {
        showAttackOptions(attacker);
        std::cout << "Your turn! Enter your attack choice (1, 2, or 3): ";
        std::cin >> choice;
        if (choice >= 1 && choice <= 3) {
            attacker->setChosenAttack(choice);
            validChoice = true;
        } else {
            std::cout << "Invalid choice, please choose 1, 2, or 3.\n";
        }
    }

    executeAttack(attacker, opponent);
}

// Função que exibe as informações do jogador
void displayPlayerStats(Player *player) {
    std::cout << "HP: " << player->getHp() << ", Attack: " << player->getAttack() << "\n";
}

void battle(Player *player1, Player *player2) {
    while (player1->getHp() > 0 && player2->getHp() > 0) {
        std::cout << "\n--- Player 1's Turn ---\n";
        playerTurn(player1, player2);
        displayPlayerStats(player1);
        displayPlayerStats(player2);

        if (player2->getHp() <= 0) {
            std::cout << "Player 1 wins!\n";
            return;
        }

        std::cout << "\n--- Player 2's Turn ---\n";
        playerTurn(player2, player1);
        displayPlayerStats(player1);
        displayPlayerStats(player2);

        if (player1->getHp() <= 0) {
            std::cout << "Player 2 wins!\n";
            return;
        }
    }
}

int main() {
    srand(time(0));

    Player *player1;
    Player *player2;

    // Menu de seleção de personagens
    int playerChoice;
    std::cout << "Player 1, choose your character (1: Warrior, 2: Rogue, 3: Mage, 4: Archer): ";
    std::cin >> playerChoice;
    switch (playerChoice) {
        case 1: player1 = new Warrior(); break;
        case 2: player1 = new Rogue(); break;
        case 3: player1 = new Mage(); break;
        case 4: player1 = new Archer(); break;
        default: player1 = new Warrior(); std::cout << "Invalid choice, defaulting to Warrior.\n"; break;
    }

    std::cout << "Player 2, choose your character (1: Warrior, 2: Rogue, 3: Mage, 4: Archer): ";
    std::cin >> playerChoice;
    switch (playerChoice) {
        case 1: player2 = new Warrior(); break;
        case 2: player2 = new Rogue(); break;
        case 3: player2 = new Mage(); break;
        case 4: player2 = new Archer(); break;
        default: player2 = new Warrior(); std::cout << "Invalid choice, defaulting to Warrior.\n"; break;
    }

    // Começo do jogo
    battle(player1, player2);

    // Final do jogo
    delete player1;
    delete player2;

    return 0;
}
