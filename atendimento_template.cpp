// ╔══════════════════════════════════════════════════════════════════════════╗
// ║  DESAFIO SQUAD — Sistema de Atendimento                                 ║
// ║  PSC II · Engenharia de Software · Campo Real · Prof. Giovane Galvão    ║
// ╠══════════════════════════════════════════════════════════════════════════╣
// ║  Compilar:  g++ -std=c++17 -Wall -o atendimento atendimento_template.cpp ║
// ║  Rodar:     ./atendimento                                                ║
// ╠══════════════════════════════════════════════════════════════════════════╣
// ║  REGRAS:                                                                 ║
// ║   • Não usar STL (vector, queue, stack, list etc.)                       ║
// ║   • Toda alocação dinâmica (new) deve ter seu delete correspondente      ║
// ║   • Commitar a cada requisito implementado (R1, R2, R3, R4)             ║
// ║   • README.md no repositório explicando como compilar e usar             ║
// ╠══════════════════════════════════════════════════════════════════════════╣
// ║  RANKING:                                                                ║
// ║   +1 por requisito que compila e funciona                                ║
// ║   +1 por commit organizado com mensagem descritiva                       ║
// ║   +2 por README.md completo                                              ║
// ║   +2 squad com melhor demo (2 min)                                       ║
// ╚══════════════════════════════════════════════════════════════════════════╝

#include <iostream>
#include <string>
using namespace std;

// ════════════════════════════════════════════════════════════════
// ESTRUTURA BASE — Nó genérico (NÃO MODIFIQUE)
// ════════════════════════════════════════════════════════════════
struct No {
    string nome;
    int    urgencia;   // 1=baixa, 2=media, 3=alta (usado apenas por Paciente)
    No*    prox;
    No(const string& n, int u = 0) : nome(n), urgencia(u), prox(nullptr) {}
};

// ════════════════════════════════════════════════════════════════
// R1 · FILA DE PACIENTES (estrutura FIFO com dois ponteiros)
// ════════════════════════════════════════════════════════════════
// Pacientes entram pelo "trás" e saem pela "frente"
// Dentro da mesma urgência, quem chegou primeiro é atendido primeiro

struct FilaPacientes {
    No* frente = nullptr;
    No* tras   = nullptr;

    // TODO 1
    void enqueue(const string& nome, int urgencia) {
        No* novo = new No(nome, urgencia);
        if (isEmpty()) {
            frente = novo;
            tras   = novo;
        } else {
            tras->prox = novo;
            tras       = novo;
        }
    }

    // TODO 2
    string dequeue() {
        if (isEmpty()) return "";
        No* temp    = frente;
        string nome = temp->nome;
        frente      = frente->prox;
        if (frente == nullptr) tras = nullptr;
        delete temp;
        return nome;
    }

    // TODO 3
    string frente_peek() const {
        if (isEmpty()) return "";
        return frente->nome;
    }

    bool isEmpty() const { return frente == nullptr; }

    // BÔNUS
    int tamanho() const {
        int cont = 0;
        for (No* p = frente; p != nullptr; p = p->prox) cont++;
        return cont;
    }

    ~FilaPacientes() {
        while (!isEmpty()) dequeue();
    }
};

// ════════════════════════════════════════════════════════════════
// R2 · HISTÓRICO COM PILHA (estrutura LIFO)
// ════════════════════════════════════════════════════════════════
// Cada atendimento concluído vai para a pilha de histórico
// O médico pode consultar o último atendimento com peek()

struct PilhaHistorico {
    No* topo = nullptr;

    // TODO 4
    void push(const string& nome) {
        No* novo   = new No(nome);
        novo->prox = topo;
        topo       = novo;
    }

    // TODO 5
    string pop() {
        if (isEmpty()) return "";
        No* temp    = topo;
        string nome = temp->nome;
        topo        = topo->prox;
        delete temp;
        return nome;
    }

    // TODO 6
    string peek() const {
        if (isEmpty()) return "";
        return topo->nome;
    }

    bool isEmpty() const { return topo == nullptr; }

    ~PilhaHistorico() {
        while (!isEmpty()) pop();
    }
};

// ════════════════════════════════════════════════════════════════
// R3 · LISTA DE MÉDICOS DISPONÍVEIS (lista encadeada)
// ════════════════════════════════════════════════════════════════
// Lista de médicos plantão. Quando chamado, médico sai da lista.
// Quando termina o atendimento, volta para o final da lista.

struct ListaMedicos {
    No* head = nullptr;

    // TODO 7
    void adicionarMedico(const string& nome) {
        No* novo = new No(nome);
        if (head == nullptr) {
            head = novo;
            return;
        }
        No* p = head;
        while (p->prox != nullptr) p = p->prox;
        p->prox = novo;
    }

    // TODO 8
    string chamarMedico() {
        if (head == nullptr) return "";
        No* temp    = head;
        string nome = temp->nome;
        head        = head->prox;
        delete temp;
        return nome;
    }

    // TODO 9
    void percorrer(const string& prefixo = "") const {
        if (!prefixo.empty()) cout << prefixo << ": ";
        if (head == nullptr) {
            cout << "(lista vazia)\n";
            return;
        }
        for (No* p = head; p != nullptr; p = p->prox) {
            cout << p->nome;
            if (p->prox != nullptr) cout << " => ";
        }
        cout << "\n";
    }

    bool temMedico() const { return head != nullptr; }

    ~ListaMedicos() {
        while (head) {
            No* t = head;
            head  = head->prox;
            delete t;
        }
    }
};

// ════════════════════════════════════════════════════════════════
// R4 · RELATÓRIO E HISTÓRICO COMPLETO
// ════════════════════════════════════════════════════════════════

// TODO 10: Implemente a função imprimirHistorico(PilhaHistorico&)
//          Imprime todos os atendimentos do histórico na ordem (do topo à base)
//          ATENÇÃO: não destrua o histórico ao imprimir!
//          Dica: use uma pilha auxiliar para preservar os dados
void imprimirHistorico(PilhaHistorico& hist) {
    cout << "\n=== Relatorio de Atendimentos ===\n";
    if (hist.isEmpty()) {
        cout << "(nenhum atendimento registrado)\n";
        return;
    }
    // ...
}

// ════════════════════════════════════════════════════════════════
// PROGRAMA PRINCIPAL — integra as 4 estruturas
// ════════════════════════════════════════════════════════════════

int main() {
    // Crie as estruturas
    FilaPacientes  fila;
    PilhaHistorico historico;
    ListaMedicos   medicos;

    // ── Cadastrar médicos (R3) ────────────────────────────────
    medicos.adicionarMedico("Dra. Ana Lima");
    medicos.adicionarMedico("Dr. Bruno Costa");
    medicos.adicionarMedico("Dra. Carla Ramos");
    medicos.percorrer("Medicos disponiveis");

    // ── Pacientes chegam na fila (R1) ─────────────────────────
    cout << "\nPacientes chegando:\n";
    fila.enqueue("Eduardo (urgencia=3)", 3);
    fila.enqueue("Fernanda (urgencia=1)", 1);
    fila.enqueue("Gabriel (urgencia=2)", 2);
    fila.enqueue("Helena (urgencia=1)", 1);

    cout << "Proximo a ser atendido: " << fila.frente_peek() << "\n";
    cout << "Total na fila: " << fila.tamanho() << "\n";

    // ── Simulação de atendimentos ─────────────────────────────
    cout << "\nSimulacao de atendimentos:\n";
    int atend = 1;
    while (!fila.isEmpty() && medicos.temMedico()) {
        string paciente = fila.dequeue();
        string medico   = medicos.chamarMedico();

        cout << "  [" << atend++ << "] " << medico
             << " atende " << paciente << "\n";

        // Registra no histórico (R2)
        historico.push(medico + " => " + paciente);

        // Médico volta para a fila depois de atender (R3)
        medicos.adicionarMedico(medico);
    }

    // ── Último atendimento (peek na pilha) ────────────────────
    cout << "\nUltimo atendimento registrado (peek): " << historico.peek() << "\n";

    // ── Relatório final (R4) ──────────────────────────────────
    imprimirHistorico(historico);

    // ── Commit sugerido ──────────────────────────────────────
    cout << "\n Lembrete de commits:\n";
    cout << "  git add atendimento_template.cpp README.md\n";
    cout << "  git commit -m \"feat: sistema de atendimento integrado (R1-R4)\"\n";
    cout << "  git push origin main\n";

    return 0;
}

// ════════════════════════════════════════════════════════════════
// CHECKLIST DE ENTREGA
// ════════════════════════════════════════════════════════════════
// [ X ] TODO 1 — enqueue funciona (paciente entra na fila)
// [ X ] TODO 2 — dequeue funciona (paciente sai da fila, memória liberada)
// [ X ] TODO 3 — frente_peek retorna o nome sem remover
// [ X ] TODO 4 — push empilha no histórico
// [ X ] TODO 5 — pop desempilha e libera memória
// [ X ] TODO 6 — peek retorna o topo sem remover
// [ X ] TODO 7 — adicionarMedico insere no final da lista
// [ X ] TODO 8 — chamarMedico remove do início e retorna o nome
// [ X ] TODO 9 — percorrer imprime todos os médicos com setas
// [ ] TODO 10 — imprimirHistorico imprime sem destruir os dados
// [ ] Programa compila sem warnings: g++ -std=c++17 -Wall -o sis main.cpp
// [ ] Sem vazamentos de memória (todo new tem delete correspondente)
// [ ] Repositório no GitHub com histórico de commits organizado
// [ ] README.md com instrução de compilação e exemplo de saída
