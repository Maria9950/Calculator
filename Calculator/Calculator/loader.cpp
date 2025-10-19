#include "loader.h"

typedef const char* (*p_name_fn)();
typedef int         (*p_arity_fn)();
typedef double      (*p_eval_fn)(const double*, int);

Loader::Loader() {}

static std::string join_path(std::string a, std::string b) {
    if (!a.empty()) {
        char last = a[a.size() - 1];
        if (last != '\\' && last != '/') a += "\\";
    }
    return a + b;
}

void Loader::load_all(std::string dir) {
    std::string pattern = join_path(dir, "*.dll");

    WIN32_FIND_DATAA data;
    HANDLE h = FindFirstFileA(pattern.c_str(), &data);
    if (h == INVALID_HANDLE_VALUE) {
        std::cerr << "[plugin] no dll found in " << dir << "\n";
        return;
    }

    do {
        std::string full = join_path(dir, data.cFileName);

        HMODULE mod = LoadLibraryA(full.c_str());
        if (!mod) {
            std::cerr << "[plugin] failed to load: " << full << "\n";
            continue;
        }

        p_name_fn  pname = (p_name_fn)GetProcAddress(mod, "plugin_name");
        p_arity_fn parity = (p_arity_fn)GetProcAddress(mod, "plugin_arity");
        p_eval_fn  peval = (p_eval_fn)GetProcAddress(mod, "plugin_eval");

        if (!pname || !parity || !peval) {
            std::cerr << "[plugin] bad dll (exports missing): " << full << "\n";
            FreeLibrary(mod);
            continue;
        }

        const char* nm = 0;
        int ar = 0;
        nm = pname();
        ar = parity();

        if (!nm || *nm == '\0') {
            std::cerr << "[plugin] empty name in: " << full << "\n";
            FreeLibrary(mod);
            continue;
        }

        Func f;
        f.module = mod;
        f.arity = ar;
        f.eval = peval;

        std::string name = nm;
        funcs_[name] = f;

        std::cerr << "[plugin] loaded '" << name
            << "' (arity=" << ar << ") from "
            << data.cFileName << "\n";

    } while (FindNextFileA(h, &data));

    FindClose(h);
}

bool Loader::has(std::string name) {
    return funcs_.find(name) != funcs_.end();
}

double Loader::call(std::string name, const std::vector<double>& args) {
    std::map<std::string, Func>::iterator it = funcs_.find(name);
    if (it == funcs_.end()) {
        throw std::runtime_error("Unknown function '" + name + "'");
    }
    Func& f = it->second;

    if ((int)args.size() != f.arity) {
        throw std::runtime_error("Arity mismatch for '" + name + "'");
    }

    const double* ptr = args.empty() ? 0 : &args[0];
    double res = f.eval(ptr, (int)args.size());
    return res;
}
