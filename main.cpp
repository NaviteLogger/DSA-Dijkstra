#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <string>
#include <tuple>

// --- PODSTAWOWE STRUKTURY I ALGORYTM ---
struct Edge {
    int to;
    long long weight;
};

struct State {
    long long cost;
    int node;

    bool operator>(const State& other) const {
        return cost > other.cost;
    }
};

void runDijkstra(int startNode, int numVertices, const std::vector<std::vector<Edge>>& adjacencyList, std::vector<long long>& distances) {
    const long long infinity = std::numeric_limits<long long>::max();
    distances.assign(numVertices + 1, infinity);
    if (startNode > numVertices || startNode <= 0) return;
    
    distances[startNode] = 0;
    std::priority_queue<State, std::vector<State>, std::greater<State>> pq;
    pq.push({0, startNode});

    while (!pq.empty()) {
        State current = pq.top();
        pq.pop();
        int u = current.node;
        long long currentCost = current.cost;
        if (currentCost > distances[u]) {
            continue;
        }
        for (const auto& edge : adjacencyList[u]) {
            if (distances[u] != infinity && distances[u] + edge.weight < distances[edge.to]) {
                distances[edge.to] = distances[u] + edge.weight;
                pq.push({distances[edge.to], edge.to});
            }
        }
    }
}


// --- SEKCJA TESTOWA ---
struct TestCase {
    std::string name;
    int numVertices;
    std::vector<std::tuple<int, int, long long>> edges;
    int startNode;
    int endNode;
    long long expectedDistance;
};

// ZMODYFIKOWANA FUNKCJA TESTUJĄCA
void runAllTests() {
    std::cout << "\n--- Uruchamianie rozszerzonego zestawu testow algorytmu Dijkstry ---\n";
    const long long infinity = std::numeric_limits<long long>::max();

    std::vector<TestCase> testsToRun = {
        {"Test 1: Prosty graf...", 4, {{1,2,10},{2,3,5},{3,4,8}}, 1, 4, 23},
        {"Test 2: Sciezka posrednia krotsza", 3, {{1,2,10},{2,3,20},{1,3,100}}, 1, 3, 30},
        {"Test 3: Brak polaczenia", 5, {{1,2,10},{4,5,10}}, 1, 5, infinity},
        {"Test 4: Graf z cyklem", 4, {{1,2,5},{2,3,5},{1,3,12},{3,4,20}}, 1, 4, 30},
        {"Test 5: Wiele sciezek", 6, {{1,2,7},{1,3,9},{1,6,14},{2,3,10},{2,4,15},{3,4,11},{3,6,2}}, 1, 6, 11},
        {"Test 6: Start = Koniec", 4, {{1,2,10}}, 3, 3, 0},
        {"Test 7: 'Pulapka' na algorytm", 4, {{1,2,10},{2,4,10},{1,3,5},{3,4,100}}, 1, 4, 20},
        {"Test 8: Krawedzie o wadze zero", 4, {{1,2,10},{2,4,10},{1,3,12},{3,4,0}}, 1, 4, 12},
        {"Test 9: Duze wagi (long long)", 4, {{1,2,1000000000},{2,3,2000000000},{3,4,3000000000LL}}, 1, 4, 6000000000LL},
        {"Test 10: Wielokrotna aktualizacja wezla", 4, {{1,4,100},{1,2,10},{2,4,20},{1,3,5},{3,4,15}}, 1, 4, 20},
        {"Test 11a: Zlozony graf (1->7)", 7, {{1,2,4},{1,3,2},{2,3,5},{2,4,10},{3,5,3},{4,5,4},{4,6,11},{5,7,8},{6,7,1}}, 1, 7, 13},
        {"Test 11b: Zlozony graf (2->6)", 7, {{1,2,4},{1,3,2},{2,3,5},{2,4,10},{3,5,3},{4,5,4},{4,6,11},{5,7,8},{6,7,1}}, 2, 6, 17},
        {"Test 11c: Zlozony graf (7->1)", 7, {{1,2,4},{1,3,2},{2,3,5},{2,4,10},{3,5,3},{4,5,4},{4,6,11},{5,7,8},{6,7,1}}, 7, 1, 13}
    };

    int passedCount = 0;
    for (const auto& test : testsToRun) {
        // WYŚWIETLANIE SCENARIUSZA
        std::cout << "\n--------------------------------------------------\n";
        std::cout << "SCENARIUSZ: " << test.name << "\n";
        std::cout << "--------------------------------------------------\n";
        std::cout << "  - Liczba wierzcholkow: " << test.numVertices << "\n";
        std::cout << "  - Krawedzie (poczatek, koniec, waga):\n";
        for (const auto& edge : test.edges) {
            std::cout << "    * (" << std::get<0>(edge) << ", " << std::get<1>(edge) << ", " << std::get<2>(edge) << ")\n";
        }
        std::cout << "  - Szukana sciezka: od " << test.startNode << " do " << test.endNode << "\n";
        std::cout << "  - Oczekiwany wynik: " << (test.expectedDistance == infinity ? "Brak sciezki" : std::to_string(test.expectedDistance)) << "\n";
        std::cout << "--------------------------------------------------\n";

        // Wykonanie testu (logika bez zmian)
        std::vector<std::vector<Edge>> adjacencyList(test.numVertices + 1);
        for (const auto& edgeTuple : test.edges) {
            adjacencyList[std::get<0>(edgeTuple)].push_back({std::get<1>(edgeTuple), std::get<2>(edgeTuple)});
            adjacencyList[std::get<1>(edgeTuple)].push_back({std::get<0>(edgeTuple), std::get<2>(edgeTuple)});
        }
        std::vector<long long> distances;
        runDijkstra(test.startNode, test.numVertices, adjacencyList, distances);
        long long actualDistance = (test.endNode > test.numVertices || test.endNode < 1) ? infinity : distances[test.endNode];
        
        // Wyświetlenie wyniku testu
        if (actualDistance == test.expectedDistance) {
            std::cout << "WYNIK: \033[1;32mPASS\033[0m\n"; // Zielony kolor
            passedCount++;
        } else {
            std::cout << "WYNIK: \033[1;31mFAIL\033[0m\n"; // Czerwony kolor
            std::cout << "   -> Otrzymano: " << (actualDistance == infinity ? "Brak sciezki" : std::to_string(actualDistance)) << "\n";
        }
    }

    std::cout << "\n==================================================\n";
    std::cout << "                  PODSUMOWANIE\n";
    std::cout << "==================================================\n";
    std::cout << "Zaliczono " << passedCount << " z " << testsToRun.size() << " testow.\n";
    if (passedCount == testsToRun.size()) {
        std::cout << "\033[1;32mWszystkie testy zakonczone sukcesem! Algorytm dziala poprawnie.\033[0m\n";
    } else {
        std::cout << "\033[1;31mNiektore testy nie powiodly sie. Sprawdz logi powyzej.\033[0m\n";
    }
    std::cout << "==================================================\n\n";
}


// --- GŁÓWNA FUNKCJA PROGRAMU Z INTERFEJSEM UŻYTKOWNIKA ---

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void calculateShortestPath() {
    int numVertices = 0, numEdges = 0;
    
    std::cout << "\n--- Obliczanie Najkrotszej Sciezki ---\n";
    std::cout << "Podaj liczbe wierzcholkow i krawedzi: ";
    while (!(std::cin >> numVertices >> numEdges) || numVertices <= 0 || numEdges < 0) {
        std::cout << "Blad. Podaj dwie dodatnie liczby calkowite: ";
        clearInputBuffer();
    }

    std::vector<std::vector<Edge>> adjacencyList(numVertices + 1);
    std::cout << "Podaj " << numEdges << " krawedzi w formacie: [wierzcholek_pocz] [wierzcholek_kon] [waga]\n";
    for (int i = 0; i < numEdges; ++i) {
        int u, v;
        long long weight;
        std::cout << "Krawedz " << i + 1 << ": ";
        while (!(std::cin >> u >> v >> weight) || u < 1 || u > numVertices || v < 1 || v > numVertices || weight < 0) {
            std::cout << "Blad: Nieprawidlowe dane. Wierzcholki musza byc w zakresie 1-" << numVertices << ", waga nieujemna. Sprobuj ponownie: ";
            clearInputBuffer();
        }
        adjacencyList[u].push_back({v, weight});
        adjacencyList[v].push_back({u, weight});
    }

    int startNode, endNode;
    std::cout << "Podaj wierzcholek poczatkowy i koncowy: ";
    while (!(std::cin >> startNode >> endNode) || startNode < 1 || startNode > numVertices || endNode < 1 || endNode > numVertices) {
        std::cout << "Blad: Nieprawidlowe numery wierzcholkow. Podaj wartosci w zakresie 1-" << numVertices << ": ";
        clearInputBuffer();
    }

    std::vector<long long> distances;
    runDijkstra(startNode, numVertices, adjacencyList, distances);

    std::cout << "\n--- WYNIK ---\n";
    const long long infinity = std::numeric_limits<long long>::max();
    if (distances[endNode] == infinity) {
        std::cout << "Nie istnieje sciezka z wierzcholka " << startNode << " do wierzcholka " << endNode << ".\n\n";
    } else {
        std::cout << "Najkrotsza odleglosc z " << startNode << " do " << endNode << " wynosi: " << distances[endNode] << "\n\n";
    }
}

int main() {
    int choice = 0;
    while (true) {
        std::cout << "=== MENU GLOWNE - ALGORYTM DIJKSTRY ===\n";
        std::cout << "1. Oblicz najkrotsza sciezke (tryb interaktywny)\n";
        std::cout << "2. Uruchom testy automatyczne\n";
        std::cout << "3. Wyjdz z programu\n";
        std::cout << "Twoj wybor: ";

        if (!(std::cin >> choice)) {
            choice = 0; 
            clearInputBuffer();
        }

        switch (choice) {
            case 1:
                calculateShortestPath();
                break;
            case 2:
                runAllTests();
                break;
            case 3:
                std::cout << "Do widzenia!\n";
                return 0;
            default:
                std::cout << "\nNieprawidlowy wybor. Sprobuj ponownie.\n\n";
                break;
        }
    }

    return 0;
}
