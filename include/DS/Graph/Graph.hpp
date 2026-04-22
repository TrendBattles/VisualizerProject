#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <DS/IDataStructure.hpp>

struct GraphNode {
    int id = 0;
    bool isActive = false;
    bool isDragging = false;
    bool isPinned = false;
    Vector2 position = Vector2{0.0f, 0.0f};
    Vector2 velocity = Vector2{0.0f, 0.0f};
    Vector2 impactForce = Vector2{0.0f, 0.0f};
};

class Graph : public IDataStructure {
public:
    Graph();
    ~Graph();

    void initDS(const std::vector <std::string>& rawValue) override;
    bool removeNode(std::string value) override;
    bool insertEdge(std::string FEdge, std::string SEdge, std::string weight) override;
    bool removeEdge(std::string FEdge, std::string SEdge) override;

    bool runDijkstra(std::string start, std::string dest) override;
    bool runKruskal() override;
    void clearAll() override;

    void update() override;
    void reset() override;
    void refreshLayout();
    void resetRefreshCounter();
    
    bool processInput(RawInputEvent nextInput) override;

    void generateSnapshot(float duration, ChangeMap changeMap = ChangeMap(), PseudocodeSection pseudoFrame = PseudocodeSection::NONE, std::vector <int> pseudoActiveLines = {}) override;
    Snapshot getCurrentSnapshot() override;
    std::string getDSID() const override;
private:
    static const int MAX_NODES = 10, inf_32 = 0x3f3f3f3f;
    static constexpr const float COOLING_CONSTANT = 0.95f, MAX_TEMP = 10000.0f, MIN_TEMP = 0.5f;
    const float radius = 30.0f, outlineSize = 6.0f;
    float temp = MAX_TEMP;
    int holdIdx = -1;

    Vector2 cameraOrigin;

    GraphNode node[MAX_NODES];
    int edgeWeight[MAX_NODES][MAX_NODES];
    int minDist[MAX_NODES], optimalPrev[MAX_NODES];

    void setEdge(int u, int v, int w);
    void insertNode(int u);
    void removeNode(int u);
    
    void Dijkstra(int u, int v);
    void Kruskal();

    Snapshot buildSnapshot(const ChangeMap& changeMap);
};
#endif