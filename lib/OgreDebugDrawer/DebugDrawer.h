/**
 * @file    DebugDrawer.cpp
 * @date    2011-12-02T01:48:06+0400
 *
 * @author  hasyimi http://ogre3d.org/tikiwiki/Debug+Drawing+Utility+Class
 */
#ifndef DEBUGDRAWER_H_INCLUDED
#define DEBUGDRAWER_H_INCLUDED

#include <OGRE/OgreSingleton.h>
#include <map>

typedef std::pair<Ogre::Vector3, Ogre::ColourValue> VertexPair;

#define DEFAULT_ICOSPHERE_RECURSION_LEVEL    1

class IcoSphere
{
public:
    struct TriangleIndices
    {
        uint v1, v2, v3;

        TriangleIndices(uint _v1, uint _v2, uint _v3) : v1(_v1), v2(_v2), v3(_v3) {}

        bool operator < (const TriangleIndices &o) const { return v1 < o.v1 && v2 < o.v2 && v3 < o.v3; }
    };

    struct LineIndices
    {
        uint v1, v2;

        LineIndices(uint _v1, uint _v2) : v1(_v1), v2(_v2) {}

        bool operator == (const LineIndices &o) const
        {
            return (v1 == o.v1 && v2 == o.v2) || (v1 == o.v2 && v2 == o.v1);
        }
    };

    IcoSphere();
    ~IcoSphere();

    void create(int recursionLevel);
    void addToLineIndices(uint baseIndex, std::list<uint> *target);
    uint addToVertices(std::list<VertexPair> *target, const Ogre::Vector3 &position, const Ogre::ColourValue &colour, float scale);
    void addToTriangleIndices(uint baseIndex, std::list<uint> *target);

private:
    uint addVertex(const Ogre::Vector3 &vertex);
    void addLineIndices(uint index0, uint index1);
    void addTriangleLines(uint index0, uint index1, uint index2);
    uint getMiddlePoint(uint index0, uint index1);
    void addFace(uint index0, uint index1, uint index2);

    void removeLineIndices(uint index0, uint index1);

    std::vector<Ogre::Vector3> vertices;
    std::list<LineIndices> lineIndices;
    std::list<int> triangleIndices;
    std::list<TriangleIndices> faces;
    std::map<uint64_t, uint> middlePointIndexCache;
    uint index;
};

class DebugDrawer
    : public Ogre::Singleton<DebugDrawer>
{
public:
    DebugDrawer(Ogre::SceneManager *_sceneManager, float _fillAlpha);
    ~DebugDrawer();

    static DebugDrawer& getSingleton(void);
    static DebugDrawer* getSingletonPtr(void);

    void build();

    void setIcoSphereRecursionLevel(int recursionLevel);

    void drawLine(const Ogre::Vector3 &start, const Ogre::Vector3 &end, const Ogre::ColourValue &colour);
    void drawCircle(const Ogre::Vector3 &centre, float radius, uint segmentsCount, const Ogre::ColourValue& colour, bool isFilled = false);
    void drawCylinder(const Ogre::Vector3 &centre, float radius, uint segmentsCount, float height, const Ogre::ColourValue& colour, bool isFilled = false);
    void drawQuad(const Ogre::Vector3 *vertices, const Ogre::ColourValue& colour, bool isFilled = false);
    void drawCuboid(const Ogre::Vector3 *vertices, const Ogre::ColourValue& colour, bool isFilled = false);
    void drawSphere(const Ogre::Vector3 &centre, float radius, const Ogre::ColourValue& colour, bool isFilled = false);
    void drawTetrahedron(const Ogre::Vector3 &centre, float scale, const Ogre::ColourValue& colour, bool isFilled = false);

    bool getEnabled() { return isEnabled; }
    void setEnabled(bool _isEnabled) { isEnabled = _isEnabled; }
    void switchEnabled() { isEnabled = !isEnabled; }

    void clear();

private:
    DebugDrawer(const DebugDrawer& obj);
    DebugDrawer& operator=(const DebugDrawer& obj);

    Ogre::SceneManager *sceneManager;
    Ogre::ManualObject *manualObject;
    float fillAlpha;
    IcoSphere icoSphere;

    bool isEnabled;

    std::list<VertexPair> lineVertices, triangleVertices;
    std::list<uint> lineIndices, triangleIndices;

    uint linesIndex, trianglesIndex;

    void initialise();
    void shutdown();

    void buildLine(const Ogre::Vector3& start, const Ogre::Vector3& end, const Ogre::ColourValue& colour, float alpha = 1.0f);
    void buildQuad(const Ogre::Vector3 *vertices, const Ogre::ColourValue& colour, float alpha = 1.0f);
    void buildFilledQuad(const Ogre::Vector3 *vertices, const Ogre::ColourValue& colour, float alpha = 1.0f);
    void buildFilledTriangle(const Ogre::Vector3 *vertices, const Ogre::ColourValue& colour, float alpha = 1.0f);
    void buildCuboid(const Ogre::Vector3 *vertices, const Ogre::ColourValue& colour, float alpha = 1.0f);
    void buildFilledCuboid(const Ogre::Vector3 *vertices, const Ogre::ColourValue& colour, float alpha = 1.0f);

    void buildCircle(const Ogre::Vector3 &centre, float radius, uint segmentsCount, const Ogre::ColourValue& colour, float alpha = 1.0f);
    void buildFilledCircle(const Ogre::Vector3 &centre, float radius, uint segmentsCount, const Ogre::ColourValue& colour, float alpha = 1.0f);

    void buildCylinder(const Ogre::Vector3 &centre, float radius, uint segmentsCount, float height, const Ogre::ColourValue& colour, float alpha = 1.0f);
    void buildFilledCylinder(const Ogre::Vector3 &centre, float radius, uint segmentsCount, float height, const Ogre::ColourValue& colour, float alpha = 1.0f);

    void buildTetrahedron(const Ogre::Vector3 &centre, float scale, const Ogre::ColourValue &colour, float alpha = 1.0f);
    void buildFilledTetrahedron(const Ogre::Vector3 &centre, float scale, const Ogre::ColourValue &colour, float alpha = 1.0f);

    uint addLineVertex(const Ogre::Vector3 &vertex, const Ogre::ColourValue &colour);
    void addLineIndices(uint index1, uint index2);

    uint addTriangleVertex(const Ogre::Vector3 &vertex, const Ogre::ColourValue &colour);
    void addTriangleIndices(uint index1, uint index2, uint index3);

    void addQuadIndices(uint index1, uint index2, uint index3, uint index4);
};

#endif
