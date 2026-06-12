// Oleksandr Tkachov 2021-2026


#include "Widgets/AbilitySystem/RPGSpellbookWidget.h"

#include "Abilities/RPGAbilityComponent.h"
#include "Abilities/RPGAbilityDefinitionData.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Widgets/AbilitySystem/RPGAbilityTooltip.h"
#include "Widgets/AbilitySystem/RPGAbilityWidget.h"

void URPGSpellbookWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	LoadAllAbilitiesFromRegistry();
	
	AbilityComponent = Cast<URPGAbilityComponent>(GetOwningPlayerPawn()->GetComponentByClass(URPGAbilityComponent::StaticClass()));
	
	TotalAbilities = AllAbilitiesInGame.Num();
	TotalPages = (TotalAbilities + (AbilitiesPerPage - 1)) / AbilitiesPerPage;
	
	PopulatePage();
	
	if (NextPageButton)
	{
		NextPageButton->OnClicked.AddDynamic(this, &URPGSpellbookWidget::OnNextPageButtonClicked);
	}
	
	if (PreviousPageButton)
	{
		PreviousPageButton->OnClicked.AddDynamic(this, &URPGSpellbookWidget::OnPreviousPageButtonClicked);
	}
	
	if (CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &URPGSpellbookWidget::OnCloseButtonClicked);
	}
}

FReply URPGSpellbookWidget::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
	
	
}

void URPGSpellbookWidget::LoadAllAbilitiesFromRegistry()
{
	IAssetRegistry& AssetRegistry =	FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry")).Get();
	
	FARFilter Filter;
	Filter.PackagePaths.Add(TEXT("/Game/InventoryProject/Core/AbilitySystem/Data/Abilities/"));
	Filter.ClassPaths.Add(URPGAbilityDefinitionData::StaticClass()->GetClassPathName());
	Filter.bRecursivePaths = true;
	Filter.bRecursiveClasses = true;

	TArray<FAssetData> FoundAssets;
	AssetRegistry.GetAssets(Filter, FoundAssets);

	UE_LOG(LogTemp, Warning, TEXT("Found %d assets at path"), FoundAssets.Num());
	
	AllAbilitiesInGame.Empty();

	for (const FAssetData& AssetData : FoundAssets)
	{
		URPGAbilityDefinitionData* Definition = Cast<URPGAbilityDefinitionData>(AssetData.GetAsset());

		if (Definition)
		{
			AllAbilitiesInGame.Add(Definition);
		}
	}
}

void URPGSpellbookWidget::PopulatePage()
{
	if (!SpellBookUniformGrid)
	{
		return;
	}
	
	if (!PageNumberText)
	{
		return;
	}
	
	SpellBookUniformGrid->ClearChildren();
	
	int32 FirstIndex = CurrentPage * AbilitiesPerPage;
	int32 LastIndex = FMath::Min(FirstIndex + AbilitiesPerPage, TotalAbilities);
	
	for (int32 i = FirstIndex; i < LastIndex - 1; i++)
	{
		URPGAbilityWidget* AbilityWidget = Cast<URPGAbilityWidget>(CreateWidget(GetWorld(), AbilityWidgetClass));
		if (!AbilityWidget)
		{
			continue;
		}
		
		if (!AllAbilitiesInGame.IsValidIndex(i))
		{
			continue;
		}
		
		AbilityWidget->AbilityDefinition = AllAbilitiesInGame[i];
		
		if (!AbilityWidget->HotkeyText)
		{
			continue;
		}
		
		AbilityWidget->HotkeyText->SetText(FText().GetEmpty());
		
		SpellBookUniformGrid->AddChildToUniformGrid(AbilityWidget, (i - FirstIndex) / GridColumns, (i - FirstIndex) % GridColumns);
		
		AbilityWidget->OnMouseHoverStart.AddDynamic(this, &URPGSpellbookWidget::OnAbilityMouseOverStart);
		AbilityWidget->OnMouseHoverEnd.AddDynamic(this, &URPGSpellbookWidget::OnAbilityMouseOverEnd);
		AbilityWidget->OnRightMouseButtonClicked.AddDynamic(this, &URPGSpellbookWidget::OnAbilityRightMouseButtonClicked);
	}
	
	const FText Text = FText::Format(FText::FromString("Page {0} / {1}"), FText::AsNumber(CurrentPage + 1), FText::AsNumber(TotalPages));
	PageNumberText->SetText(Text);
	
	UpdateButtons();
}

void URPGSpellbookWidget::UpdateButtons()
{
	if (!NextPageButton)
	{
		return;
	}
	
	if (!PreviousPageButton)
	{
		return;
	}
	
	NextPageButton->SetIsEnabled(CurrentPage < TotalPages - 1);
	PreviousPageButton->SetIsEnabled(CurrentPage > 0);
}

void URPGSpellbookWidget::OnNextPageButtonClicked()
{
	CurrentPage++;
	PopulatePage();
}

void URPGSpellbookWidget::OnPreviousPageButtonClicked()
{
	CurrentPage--;
	PopulatePage();
}

void URPGSpellbookWidget::OnCloseButtonClicked()
{
	// TODO: Do better logic
	if (!GetOwningPlayer())
	{
		return;
	}
	
	GetOwningPlayer()->SetInputMode(FInputModeGameOnly());
	GetOwningPlayer()->bShowMouseCursor = false;
	RemoveFromParent();
}

void URPGSpellbookWidget::OnAbilityMouseOverStart(URPGAbilityDefinitionData* AbilityDefinition)
{
	if (!AbilityDefinition)
	{
		return;
	}
	
	if (!AbilityTooltipWidget)
	{
		return;
	}
	
	AbilityTooltipWidget->SetVisibility(ESlateVisibility::Visible);
	AbilityTooltipWidget->SetupTooltip(AbilityDefinition);
}

void URPGSpellbookWidget::OnAbilityMouseOverEnd(URPGAbilityDefinitionData* AbilityDefinition)
{
	if (!AbilityTooltipWidget)
	{
		return;
	}
	
	AbilityTooltipWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void URPGSpellbookWidget::OnAbilityRightMouseButtonClicked(URPGAbilityDefinitionData* AbilityDefinition)
{
	if (!AbilityComponent)
	{
		return;
	}
	
	AbilityComponent->AddAbility(AbilityDefinition);
}
