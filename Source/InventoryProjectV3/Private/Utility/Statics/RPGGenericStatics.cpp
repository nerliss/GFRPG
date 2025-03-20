// Oleksandr Tkachov 2022-2025


#include "Utility/Statics/RPGGenericStatics.h"

#include "DesktopPlatform/Public/DesktopPlatformModule.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"

URPGGenericStatics::URPGGenericStatics()
{

}

UTexture2D* URPGGenericStatics::LoadTextureFromFile(const FString& ImagePath)
{
	TArray<uint8> FileData;
	if (!FFileHelper::LoadFileToArray(FileData, *ImagePath))
	{
		return nullptr;
	}

	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));

	// Detect the file format and create an appropriate ImageWrapper
	const EImageFormat ImageFormat = ImageWrapperModule.DetectImageFormat(FileData.GetData(), FileData.Num());
	const TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(ImageFormat);

	if (!ImageWrapper.IsValid() || !ImageWrapper->SetCompressed(FileData.GetData(), FileData.Num()))
	{
		return nullptr; 
	}

	const int32 Width = ImageWrapper->GetWidth();
	const int32 Height = ImageWrapper->GetHeight();

	TArray<uint8> RawData;
	if (!ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, RawData))
	{
		return nullptr; 
	}

	UTexture2D* LoadedTexture = UTexture2D::CreateTransient(Width, Height);
	if (!LoadedTexture)
	{
		return nullptr;
	}

	LoadedTexture->SRGB = true;
	LoadedTexture->Filter = TF_Default;
	LoadedTexture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);

	void* TextureData = LoadedTexture->GetPlatformData()->Mips[0].BulkData.Realloc(RawData.Num());
	FMemory::Memcpy(TextureData, RawData.GetData(), RawData.Num());

	LoadedTexture->GetPlatformData()->Mips[0].BulkData.Unlock();
	LoadedTexture->UpdateResource();

	return LoadedTexture;
}

void URPGGenericStatics::ApplyTextureToMaterial(UTexture2D* NewTexture, UMaterialInstanceDynamic* DynamicMat)
{
	if (!DynamicMat || !NewTexture)
	{
		return;
	}

	DynamicMat->SetTextureParameterValue(FName("RuntimeTextureParam"), NewTexture);
}

FString URPGGenericStatics::OpenFileDialog()
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (!DesktopPlatform)
	{
		return FString();
	}

	TArray<FString> SelectedFiles;
	const bool bOpened = DesktopPlatform->OpenFileDialog(
		nullptr, 
		TEXT("Select an Image you want to apply"), 
		FPaths::RootDir(), 
		TEXT(""), 
		TEXT("Image Files (*.png;*.jpg;*.bmp)|*.png;*.jpg;*.bmp"), 
		EFileDialogFlags::None, 
		SelectedFiles
	);

	return (bOpened && SelectedFiles.Num() > 0) ? SelectedFiles[0] : FString();
}

void URPGGenericStatics::ApplyMaterialToMesh(UStaticMeshComponent* MeshComponent, UMaterialInterface* BaseMaterial)
{
	if (!MeshComponent || !BaseMaterial)
	{
		UE_LOG(LogTemp, Error, TEXT("MeshComponent or BaseMaterial is nullptr"));
		return;
	}

	const FString ImagePath = OpenFileDialog(); 
	if (ImagePath.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("No image selected"));
		return;
	}

	UTexture2D* LoadedTexture = LoadTextureFromFile(ImagePath);
	if (!LoadedTexture) 
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load a texture from file"));
		return;
	}

	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, MeshComponent);
	MeshComponent->SetMaterial(0, DynamicMaterial);

	ApplyTextureToMaterial(LoadedTexture, DynamicMaterial);
}
