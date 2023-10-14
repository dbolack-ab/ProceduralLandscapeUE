//Copyright Maxime Dupart 2021  https://twitter.com/Max_Dupt

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GeometryClipMapWorld.generated.h"

class UProceduralMeshComponent;
class UMaterialInterface;
class UMaterialInstanceDynamic;
class UTextureRenderTarget2D;
class UStaticMesh;
class UHierarchicalInstancedStaticMeshComponent;
class UGeoClipmapMeshComponent;
class UInstancedStaticMeshComponent;
class UMaterialParameterCollection;
class UTextureRenderTarget2DArray;

UENUM(BlueprintType)
enum class EGeoClipWorldType : uint8
{
	FlatWorld UMETA(DisplayName = "Flat world generation"),
	SphericalWorld UMETA(DisplayName = "Spherical world generation"),
};

UENUM(BlueprintType)
enum class EWorldPresentation : uint8
{
	Smooth UMETA(DisplayName = "Triangle Based Terrain"),
	InstancedMesh UMETA(DisplayName = "InstancedMesh Shaped"),
};

UENUM(BlueprintType)
enum class EClipMapInteriorConfig : uint8
{
	BotLeft UMETA(DisplayName = "BotLeft"),
	TopLeft UMETA(DisplayName = "TopLeft"),
	BotRight UMETA(DisplayName = "BotRight"),
	TopRight UMETA(DisplayName = "TopRight"),
	NotVisible UMETA(DisplayName = "NotVisible"),
};

UENUM(BlueprintType)
enum class ENValue : uint8
{
	N511 UMETA(DisplayName = "511"),
	N255 UMETA(DisplayName = "255"),
	N127 UMETA(DisplayName = "127"),
	N63 UMETA(DisplayName = "63"),
	N31 UMETA(DisplayName = "31"),
	N15 UMETA(DisplayName = "15"),
};

USTRUCT()
struct FClipMapMeshElement
{
	GENERATED_BODY()

	UPROPERTY(Transient)
		UGeoClipmapMeshComponent* Mesh = nullptr;

	//In case we're using instancedMesh for rendering
	UPROPERTY(Transient)
		UInstancedStaticMeshComponent* I_Mesh = nullptr;

	UPROPERTY(Transient)
		float GridSpacing = 1.f;
	UPROPERTY(Transient)
		int Level = 0;

	UPROPERTY(Transient)
		FVector Location=FVector::ZeroVector;

	UPROPERTY(Transient)
		EClipMapInteriorConfig Config = EClipMapInteriorConfig::BotLeft;
	UPROPERTY(Transient)
		UMaterialInstanceDynamic* MatDyn=nullptr;


	UPROPERTY(Transient)
		UTextureRenderTarget2D* HeightMap = nullptr;
	UPROPERTY(Transient)
		UTextureRenderTarget2D* NormalMap = nullptr;

	UPROPERTY(Transient)
		UMaterialInstanceDynamic* CacheMatDyn = nullptr;

	/*
	// Would need new further developement | Sticking to the RT array for ease to implement
	UPROPERTY(Transient)
		UTextureRenderTarget2DArray* LandLayers = nullptr;
	*/
	UPROPERTY(Transient)
		TArray<UTextureRenderTarget2D*> LandLayers;
	UPROPERTY(Transient)
		UMaterialInstanceDynamic* LayerMatDyn = nullptr;

	

	UPROPERTY(Transient)
	TArray<bool> SectionVisibility;

	bool IsSectionVisible(int SectionID);
	void SetSectionVisible(int SectionID,bool NewVisibility);
};

USTRUCT(BlueprintType)
struct FClipMapLayer
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
		FString LayerName = "LayerName";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawnables")
		UMaterialInterface* MaterialToGenerateLayer = nullptr;
	UPROPERTY(Transient)
		int ID=0;
};
USTRUCT()
struct FCollisionMeshElement
{
	GENERATED_BODY()

	UPROPERTY(Transient)
		UProceduralMeshComponent* Mesh = nullptr;
	UPROPERTY(Transient)
		UTextureRenderTarget2D* CollisionRT = nullptr;
	UPROPERTY(Transient)
		FVector Location=FVector::ZeroVector;
	UPROPERTY(Transient)
		int ID=0;
	UPROPERTY(Transient)
	TArray<FColor> HeightData;

};

USTRUCT()
struct FMeshElementOffset
{
	GENERATED_BODY()

	UPROPERTY(Transient)
		UProceduralMeshComponent* Mesh = nullptr;
	UPROPERTY(Transient)
		FVector Offset=FVector::ZeroVector;

};

USTRUCT()
struct FInstanceIndexes
{
	GENERATED_BODY()

		UPROPERTY(Transient)
		TArray<int> InstancesIndexes;
};


USTRUCT()
struct FSpawnableMeshElement
{
	GENERATED_BODY()

	UPROPERTY(Transient)
		UTextureRenderTarget2D* LocationX = nullptr;
	UPROPERTY(Transient)
		UTextureRenderTarget2D* LocationY = nullptr;
	UPROPERTY(Transient)
		UTextureRenderTarget2D* LocationZ = nullptr;
	UPROPERTY(Transient)
		UTextureRenderTarget2D* Rotation = nullptr;

	UPROPERTY(Transient)
		UMaterialInstanceDynamic* ComputeSpawnTransformDyn=nullptr;

	UPROPERTY(Transient)
		FVector Location=FVector::ZeroVector;
	UPROPERTY(Transient)
		int ID=0;

	UPROPERTY(Transient)
		TArray<FColor> LocationXData;
	UPROPERTY(Transient)
		TArray<FColor> LocationYData;
	UPROPERTY(Transient)
		TArray<FColor> LocationZData;
	
	UPROPERTY(Transient)
		TArray<FColor> RotationData;	
	

	UPROPERTY(Transient)
		TArray<FTransform> InstancesTransform;

	UPROPERTY(Transient)
		TArray<FInstanceIndexes /*TArray<int>*/> InstancesIndexes;

	UPROPERTY(Transient)
		TArray<int> InstanceOffset;
	UPROPERTY(Transient)
		int InstanceCount=0;

};

class AGeometryClipMapWorld;

USTRUCT(BlueprintType)
struct FSpawnableMesh
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshToSpawn")
		bool CollisionEnabled = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshToSpawn")
		bool CastShadows = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshToSpawn")
		TArray<UStaticMesh*> Mesh;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshToSpawn")
		float AlignMaxAngle = 90.f;

	UPROPERTY(EditAnywhere, Category = "MeshToSpawn")
		FFloatInterval AltitudeRange = FFloatInterval(-10000000.f,10000000.f);

	UPROPERTY(EditAnywhere, Category = "MeshToSpawn")
		FFloatInterval ScaleRange = FFloatInterval(.75f,1.25f);

	UPROPERTY(EditAnywhere, Category = "MeshToSpawn")
		FFloatInterval GroundSlopeAngle = FFloatInterval(0.f, 45.f);

	UPROPERTY(Transient)
		TArray<UHierarchicalInstancedStaticMeshComponent*> HIM_Mesh;
	UPROPERTY(Transient)
		TArray<int> InstanceIndexToHIMIndex;
	UPROPERTY(Transient)
		TArray<int> NumInstancePerHIM;
	UPROPERTY(Transient)
		TArray<int> InstanceIndexToIndexForHIM;

	UPROPERTY(Transient)
		AGeometryClipMapWorld* Owner = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision Settings")
		int NumberOfInstanceToComputePerRegion = 65;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision Settings")
		float RegionWorldDimension = 6400.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision Settings")
		int RT_Dim = 30;

	UPROPERTY(Transient)
		TArray<FSpawnableMeshElement> SpawnablesElem;
	UPROPERTY(Transient)
		TArray<int> AvailableSpawnablesElem;
	UPROPERTY(Transient)
		TArray<int> UsedSpawnablesElem;
	UPROPERTY(Transient)
		TArray<int> SpawnablesElemReadToProcess;

	UPROPERTY(Transient)
		TMap<FIntVector, int /*FSpawnableMeshElement*/> SpawnablesLayout;

	UPROPERTY(Transient)
		int IndexOfClipMapForCompute = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawnables")
		UMaterialInterface* CustomSpawnablesMat=nullptr;
	FSpawnableMeshElement& GetASpawnableElem();
	void ReleaseSpawnableElem(int ID);

	void UpdateSpawnableData(FSpawnableMeshElement& MeshElem );

	void Initiate(AGeometryClipMapWorld* Owner_);

	void CleanUp();

	~FSpawnableMesh();
};


UCLASS()
class PROCEDURALLANDSCAPE_API AGeometryClipMapWorld : public AActor
{
	GENERATED_UCLASS_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGeometryClipMapWorld();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:


#if WITH_EDITOR

	bool ShouldTickIfViewportsOnly() const override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

#endif

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClipMap Settings")
		float UpdateRatePerSecond = 20.0f;
	/*Number of texel we're storing in between each vertices, with 4 texels, 255 vertices and 12 LODs, it's 50MB of video ram*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClipMap Settings")
		int ClipMapCacheIntraVerticesTexel=4;

	UPROPERTY(Transient)
		float TimeAcu = 0.0f;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClipMap Settings")
		bool GenerateCollision = false;

	/*If defining a static landscape, cache the landscape computation instead of computing it each frame*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClipMap Settings")
		bool EnableCaching = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClipMap Settings",meta = (EditCondition = "EnableCaching"))
		int LOD_above_doubleCacheResolution = 2;

	UPROPERTY(Transient,EditAnywhere, BlueprintReadWrite, Category = "ClipMap Settings")
		bool rebuild=false;
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = "Spawnables")
		bool rebuildVegetationOnly = false;

	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = "ClipMap Settings")
		bool debug = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClipMap Settings")
		int GridSpacing = 5000;

	/*Valid Value for N: 255/127/63/31/15 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ClipMap Settings")
		int N = 255;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClipMap Settings")
		ENValue N_Select = ENValue::N255;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClipMap Settings")
		int Level = 8;
	/*Hack the culling of the landscape with vertical noise*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClipMap Hack Culling")
		float VerticalRangeMeters = 0.f;

	UPROPERTY(/*EditAnywhere, BlueprintReadWrite, Category = "ClipMap Settings"*/)
		EGeoClipWorldType WorldType = EGeoClipWorldType::FlatWorld;
	UPROPERTY(/*EditAnywhere, BlueprintReadWrite, Category = "ClipMap Settings"*/)
		EWorldPresentation WorldPresentation = EWorldPresentation::Smooth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClipMap Settings")
		UMaterialInterface* Mat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClipMap Settings")
		UMaterialInterface* MatwCaching;
	UPROPERTY(Transient)
		UMaterialInstanceDynamic* MatDyn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClipMap Settings")
		UMaterialInterface* CacheMat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClipMap Settings")
		TArray<FClipMapLayer> LandDataLayers;
	UPROPERTY(Transient)
		UMaterialInstanceDynamic* CacheMatDyn;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision Settings")
		int CollisionMeshPerQuadrantAroundPlayer = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision Settings")
		int CollisionMeshVerticeNumber = 65;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision Settings")
		float CollisionMeshWorldDimension = 6400.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision Settings")
		UMaterialInterface* CollisionMat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision Settings")
		UMaterialInterface* CollisionMat_HeightRead;

	UPROPERTY(Transient)
		float TimeAcuSpawnable = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawnables")
		float UpdateRateSpanablesPerSecond = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawnables")
		TArray<FSpawnableMesh> Spawnables;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawnables")
		UMaterialInterface* SpawnablesMat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawnables")
		int DrawCallBudget_Spawnables = 3;
	/*Relevant Only if using InstancedMesh representation*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClipMap WorldPresentation")
	UStaticMesh* VisualRepresentation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClipMap WorldPresentation")
		UMaterialInterface* Material_InstancedMeshRepresentation;

	int GetMeshNum(){return Meshes.Num();};
	FClipMapMeshElement& GetMesh(int i){return Meshes[i];};

protected:

	UPROPERTY(Transient)
		TArray<FClipMapMeshElement> Meshes;

	UPROPERTY(Transient)
		TArray<FMeshElementOffset> MeshesOffset;

	UPROPERTY(Transient)
		TArray<FCollisionMeshElement> CollisionMesh;	
	UPROPERTY(Transient)
		TArray<int> AvailableCollisionMesh;
	UPROPERTY(Transient)
		TArray<int> UsedCollisionMesh;

	UPROPERTY(Transient)
		TArray<int> CollisionReadToProcess;

	UPROPERTY(Transient)
		TMap<FIntVector,FCollisionMeshElement> GroundCollisionLayout;

	FCollisionMeshElement& GetACollisionMesh();
	void ReleaseCollisionMesh(int ID);

	void Setup();
	void InitiateWorld();
	void SetN();
	void CreateGridMeshWelded(int LOD, int32 NumX, int32 NumY, TArray<int32>& Triangles, TArray<FVector>& Vertices, TArray<FVector2D>& UVs,TArray<FVector2D>& UV1s,TArray<FVector2D>& UV2s, float& GridSpacing, FVector& Offset, uint8 StitchProfil);
	void UpdateCameraLocation();
	float HeightToClosestCollisionMesh();
	void UpdateClipMap();
	void UpdateCollisionMesh();
	void UpdateSpawnables();
	bool CanUpdateSpawnables();

	double GetHeightFromGPURead(FColor& ReadLoc);
	void ProcessCollisionsPending();
	double ComputeWorldHeightAt(FVector WorldLocation);
	void UpdateCollisionMeshData(FCollisionMeshElement& Mesh );

	FTransform GetWorldTransformOfSpawnable(const FVector& CompLoc, FColor& LocX, FColor& LocY, FColor& LocZ, FColor& Rot);
	void ProcessSpawnablePending();

	EClipMapInteriorConfig RelativeLocationToParentInnerMeshConfig(FVector RelativeLocation);

	void UpdateParentInnerMesh(int ChildLevel, EClipMapInteriorConfig NewConfig);
	FVector CamLocation;

	FVector LastValidL0;
	bool LastValidSet=false;
	FRenderCommandFence RTUpdate;

	bool GenerateCollision_last = false;
	float VerticalRangeMeters_last = 0.f;
	bool Caching_last=false;

	int DrawCall_Spawnables_count = 0;
	UStaticMesh* Spawnable_Stopped = nullptr; 
	

	//TArray<FVector> CollisionMesh_ReferenceVertices;
	//TArray<int32> CollisionMesh_ReferenceIndices;


};
