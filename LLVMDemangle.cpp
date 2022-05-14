#include <cstdio>
#include <cstdlib>

#include <LLVMDemangle.h>

#include <llvm/Demangle/Demangle.h>

// This code is adapted from llvm/lib/Demangle/Demangle.cpp

static bool isItaniumEncoding(const char* S) {
    // Itanium encoding requires 1 or 3 leading underscores, followed by 'Z'.
    return std::strncmp(S, "_Z", 2) == 0 || std::strncmp(S, "___Z", 4) == 0;
}

static bool isRustEncoding(const char* S) { return S[0] == '_' && S[1] == 'R'; }

static bool isDLangEncoding(const std::string& MangledName) {
    return MangledName.size() >= 2 && MangledName[0] == '_' &&
        MangledName[1] == 'D';
}

static char* nonMicrosoftDemangle(const char* MangledName) {
    char* Demangled = nullptr;
    if (isItaniumEncoding(MangledName))
        Demangled = llvm::itaniumDemangle(MangledName, nullptr, nullptr, nullptr);
    else if (isRustEncoding(MangledName))
        Demangled = llvm::rustDemangle(MangledName, nullptr, nullptr, nullptr);
    else if (isDLangEncoding(MangledName))
        Demangled = llvm::dlangDemangle(MangledName);

    return Demangled;
}

char* LLVMDemangle(const char* MangledName)
{
    char* Demangled = nullptr;

    if ((Demangled = nonMicrosoftDemangle(MangledName)))
        return Demangled;

    if (MangledName[0] == '_' && (Demangled = nonMicrosoftDemangle(MangledName + 1)))
        return Demangled;

    if ((Demangled = llvm::microsoftDemangle(MangledName, nullptr, nullptr, nullptr, nullptr)))
        return Demangled;

    return nullptr;
}

void LLVMDemangleFree(char* DemangledName)
{
    std::free(DemangledName);
}
