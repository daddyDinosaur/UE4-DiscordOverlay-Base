#pragma once
#define CONCAT_IMPL(x, y) x##y
#define MACRO_CONCAT(x, y) CONCAT_IMPL(x, y)
#define PAD(SIZE) BYTE MACRO_CONCAT(_pad, __COUNTER__)[SIZE];

#include <Windows.h>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <locale>
#include <array>
#include "UE4/UE4.h"

template<class T>
class TArray
{
	friend struct FString;
public:
	TArray() { Count = Max = 0; };

	T& operator[](int i) const { return Data[i]; };

	T* Data;
	int Count; // int32_t
	int Max; // int32_t
};

struct FNameEntry
{
	uint32_t Index;
	uint32_t Pad;
	FNameEntry* HashNext;

	union
	{
		char AnsiName[1024];
		wchar_t WideName[1024];
	};
	const int GetIndex() const { return Index >> 1; }
	const char* GetAnsiName() const { return AnsiName; }
};

class TNameEntryArray
{
public:
	bool IsValidIndex(uint32_t index) const { return index < NumElements; }

	FNameEntry const* GetByID(uint32_t index) const { return *GetItemPtr(index); }

	FNameEntry const* const* GetItemPtr(uint32_t Index) const
	{
		const auto ChunkIndex = Index / 16384;
		const auto WithinChunkIndex = Index % 16384;
		const auto Chunk = Chunks[ChunkIndex];

		return Chunk + WithinChunkIndex;
	}

	FNameEntry** Chunks[128]; // ChunkTableSize
	uint32_t NumElements = 0;
	uint32_t NumChunks = 0;
};

struct FName
{
	int ComparisonIndex = 0;
	int Number = 0;

	static inline TNameEntryArray* GNames = nullptr;

	static const char* GetNameByIDFast(int ID)
	{
		auto NameEntry = GNames->GetByID(ID);
		if (!NameEntry) return nullptr;
		return NameEntry->AnsiName;
	}
	static std::string GetNameByID(int ID)
	{
		auto NameEntry = GNames->GetByID(ID);
		if (!NameEntry) return std::string();
		return NameEntry->AnsiName;
	}
	const char* GetNameFast() const
	{
		auto NameEntry = GNames->GetByID(ComparisonIndex);
		if (!NameEntry) return nullptr;
		return NameEntry->AnsiName;
	}
	const std::string GetName() const
	{
		auto NameEntry = GNames->GetByID(ComparisonIndex);
		if (!NameEntry) return std::string();
		return NameEntry->AnsiName;
	}
	inline bool operator==(const FName& other) const
	{
		return ComparisonIndex == other.ComparisonIndex;
	}

	FName() {}
	FName(const char* nameToFind)
	{
		for (int i = 1000u; i < GNames->NumElements; i++)
		{
			auto Name = GetNameByIDFast(i);
			if (!Name) continue;
			if (strcmp(Name, nameToFind) == 0)
			{
				ComparisonIndex = i;
				return;
			}
		}
	}
};

struct FMinimalViewInfo {
	FVector Location; //0x00(0xC)
	FRotator Rotation; //0xC(0xC)
	PAD(0x10); 
	float FOV; //0x28(0XC)
};