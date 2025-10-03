#include <plog/Log.h>
#include <nlohmann/json.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <unordered_map>
#include "model.h"
#include "assetmanager.h"

using nlohmann::json;

Model::Model(MeshPtr msh, const std::vector<ModelMeshPart>& mshParts):
	mesh(msh), meshParts(mshParts)
{
}

Model::~Model()
{
}


ModelAsset::ModelAsset(ModelPtr model)
{
}

ModelAsset::~ModelAsset()
{
}

ModelAssetLoader::~ModelAssetLoader()
{
}

std::optional<std::shared_ptr<IAsset>> ModelAssetLoader::Load(const AssetURI& uri)
{
	PLOGV << "Loading model asset from URI '" << uri << "'";

	auto& am = AssetManager::GetInstance();
	auto data = am.GetData(uri);
	
	if (!data.has_value())
	{
		return std::nullopt;
	}

	json modelRecipe = json::parse(data.value());

	if (!modelRecipe.contains("mesh"))
	{
		PLOGE << "Model recipe '" << uri << "' is missing mesh URI";
		return std::nullopt;
	}

	if (!modelRecipe.contains("materials"))
	{
		PLOGE << "Model recipe '" << uri << "' is missing materials list";
		return std::nullopt;
	}

	auto materialsList = modelRecipe["materials"];

	if (!materialsList.contains("*"))
	{
		PLOGE << "Model recipe '" << uri << "' is missing fallback material URI";
		return std::nullopt;
	}

	auto fallbackMaterialURI = AssetURI(materialsList["*"]);
	auto fallbackMaterialHandle = am.Require<MaterialAsset>(fallbackMaterialURI);
	
	if (!fallbackMaterialHandle)
	{
		return std::nullopt;
	}

	AssetURI modelMeshURI(modelRecipe["mesh"]);
	auto meshData = am.GetData(modelMeshURI);

	if (!meshData.has_value())
	{
		PLOGE << "No data for mesh from URI '" << modelMeshURI << "'";
		return std::nullopt;
	}

	auto meshDataRaw = (*meshData).str();
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFileFromMemory(
		meshDataRaw.data(), 
		meshDataRaw.size(),
		aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType | aiProcess_OptimizeMeshes | aiProcess_GenSmoothNormals,
		"obj"
	);

	if (!scene || !scene->mRootNode)
	{
		PLOGE << "Mesh from URI '" << modelMeshURI << "' can't be imported: " << importer.GetErrorString();
		return std::nullopt;
	}

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<MeshPart> meshParts;
    std::vector<ModelMeshPart> modelParts;

	GLint baseVertex = 0;
	size_t baseIndex = 0;
	size_t modelPartIndex = 0;

	for (size_t i = 0; i < scene->mNumMeshes; i++)
	{
		const aiMesh* objMesh = scene->mMeshes[i];

		bool hasNormals = objMesh->HasNormals();
		bool hasUVs = objMesh->HasTextureCoords(0);

		for (size_t j = 0; j < objMesh->mNumVertices; j++)
		{
			Vertex vtx;

			const auto& coord = objMesh->mVertices[j];
			vtx.coords = glm::vec3(coord.x, coord.y, coord.z);

			if (hasNormals)
			{
				const auto& norm = objMesh->mNormals[j];
				vtx.normal = glm::vec3(norm.x, norm.y, norm.z);
			}
			else
			{
				vtx.normal = glm::vec3(0.f, 0.f, 0.f);
			}

			if (hasUVs)
			{
				const auto& uv = objMesh->mTextureCoords[0][j];
				vtx.textureCoords = glm::vec2(uv.x, uv.y);
			}
			else
			{
				vtx.textureCoords = glm::vec2(0.f, 0.f);
			}

			vertices.push_back(vtx);
		}

		size_t numIndices = 0;

		for (size_t j = 0; j < objMesh->mNumFaces; j++)
		{
			const auto& face = objMesh->mFaces[j];
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);

			numIndices += 3;
		}

		MeshPart meshPart;
		meshPart.numIndices = numIndices;
		meshPart.baseIndex = baseIndex;
		meshPart.baseVertex = baseVertex;
		meshParts.push_back(meshPart);

		auto matIndex = objMesh->mMaterialIndex;
		auto objMaterial = scene->mMaterials[matIndex];
		auto objMaterialName = objMaterial->GetName().C_Str();
		auto objMatHandle = fallbackMaterialHandle;

		if (materialsList.contains(objMaterialName))
		{
			objMatHandle = am.Require<MaterialAsset>(AssetURI(materialsList[objMaterialName]));
		}

		ModelMeshPart modelPart;
		modelPart.material = objMatHandle;
		modelPart.partIndex = modelPartIndex;
		modelParts.push_back(modelPart);

		modelPartIndex++;
		baseVertex += vertices.size();
		baseIndex += indices.size();
	}

    MeshPtr mesh = std::make_shared<Mesh>(vertices, indices, meshParts);

    if (!mesh->IsValid())
	{
        PLOGE << "Mesh from URI '" << modelMeshURI << "' failed to create on GPU";
        return std::nullopt;
    }

	PLOGV << "Model asset from URI '" << uri << "' OK";

    auto model = std::make_shared<Model>(mesh, modelParts);
    auto asset = std::make_shared<ModelAsset>(model);
    return std::static_pointer_cast<IAsset>(asset);
}
