// =============================================================================
// Utils | EdgeDetection
// =============================================================================

#include <stdint.h>

#ifndef UTILS_EDGE_DETECTION_
#define UTILS_EDGE_DETECTION_

class EdgeDetection
{
private:
    uint8_t *_edgeValue;
    static EdgeDetection *edgeList[10]; //* Has to be adjusted to the number of instances of EdgeDetection
    static uint8_t edgeCount;

    struct edge_t
    {
        uint8_t old;
        uint8_t act;
        uint8_t edge;
        uint8_t edge_pos;
        uint8_t edge_neg;
    } Edge;

public:
    /// @brief EdgeDetection constructor
    ///
    /// @param edgeValue Pointer to the variable representing the edge value
    ///
    EdgeDetection(uint8_t *edgeValue);

    /// @brief Update the edge detection for a specific instance
    ///
    void updateEdge();

    /// @brief Update the edge detection for all instances of EdgeDetection
    ///
    static void updateEdges();

    /// @brief Get the previous state value
    ///
    /// @return The previous state value
    ///
    uint8_t getOldState() { return Edge.old; };

    /// @brief Get the current state value
    ///
    /// @return The current state value
    ///
    uint8_t getActState() { return Edge.act; }

    /// @brief Get the edge value
    ///
    /// @return The edge value
    ///
    uint8_t getEdge() { return Edge.edge; }

    /// @brief Get the positive edge value
    ///
    /// @return The positive edge value
    ///
    uint8_t getEdgePos() { return Edge.edge_pos; }

    /// @brief Get the negative edge value
    ///
    /// @return The negative edge value
    ///
    uint8_t getEdgeNeg() { return Edge.edge_neg; }
};

#endif // UTILS_EDGE_DETECTION_
