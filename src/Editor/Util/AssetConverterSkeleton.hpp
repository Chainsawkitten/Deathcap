#include <string>

class AssetConverterSkeleton {
public:
    /// Convert a mesh from fbx to .asset format.
    /**
    * @param filepath Filepath of the fbxfile.
    * @param destination Filepath of the destination inculding name and extension.
    * @param triangulate Should the mesh be triangulated?
    * @param importNormals Should normals be imported from the mesh?
    * @param importTangents Should tangents be imported from the mesh?
    */
    bool Convert(const char * filepath, const char * destination,
        bool triangulate, bool importNormals, bool importTangents);

    /// Check after conversion if everything went well.
    /**
    * @return True if successfully converted model.
    */
    bool Success() const;

    /// String with errors and warnings.
    /**
    * @return A string with messages of what went wrong durning conversion.
    */
    std::string& GetErrorString();

private:

};