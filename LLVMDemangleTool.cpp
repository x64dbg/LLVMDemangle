#include <cstdio>
#include <cstdlib>

#include <LLVMDemangle.h>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        puts("Usage: LLVMDemangleTool <mangled name>");
        return EXIT_FAILURE;
    }
    auto demangled = LLVMDemangle(argv[1]);
    if (!demangled)
    {
        printf("Invalid mangled name '%s'\n", argv[1]);
        return EXIT_FAILURE;
    }
    puts(demangled);
    LLVMDemangleFree(demangled);
    return EXIT_SUCCESS;
}