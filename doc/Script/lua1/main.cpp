#include <stdio.h>
#include <stdlib.h>
#include <lua/lua.hpp>

int helloWorld(lua_State* L)
{
	printf("hello world/n");
	return 0;
}

int affichage(lua_State* L)
{
	int nbArguments = lua_gettop(L);
	if (nbArguments != 1)
	{
		fprintf(stderr,"nombre d'arguments invalide");
		return 0;
	}
	printf("parametre : %s\n",lua_tostring(L,1));
	return 0;
}

int printParams(lua_State* L)
{
	printf("affichage des parametres de la fonction : \n");;
	int nbParams = lua_gettop(L);
	// Attention : lua commence à compter à partir de 1
	for (int i = 1; i <= nbParams; i++)
	{
		if (lua_isnumber(L,i))
		{
			printf("%f\n",lua_tonumber(L,i));
		}
		else if (lua_isboolean(L,i))
		{
			if (lua_toboolean(L,i))
			{
				printf("true\n");;
			}
			else
			{
				printf("false\n");
			}
		}
		else if (lua_isstring(L,i))
		{
			printf("%s\n",lua_tostring(L,i));
		}
	}
	return 0;
}

int halfParam(lua_State* L)
{
	int nbParam = lua_gettop(L);
	if (nbParam != 1)
	{
		printf("halfParam : mauvais nombre de parametres\n");
		return 0;
	}
	if (!lua_isnumber(L,1))
	{
		printf("halfParam : le parametre doit etre de type number\n");
		return 0;
	}
	float param = lua_tonumber(L,1);
	float ret = param / 2.0f;
	lua_pushnumber(L,ret);
	return 1;
}

int main(int argc, char** argv)
{
	lua_State * state;
	// on crée un context d'execution de Lua
	state = lua_open();
	// on charge les librairies standards de Lua
	luaL_openlibs(state);

	// on enregistre nos fonction C++ pour qu'elle puissent
	// etre accedée depuis Lua
	lua_register(state,"helloWorld",helloWorld);
	lua_register(state,"affiche",affichage);
	lua_register(state,"printParams",printParams);
	lua_register(state,"halfParam",halfParam);

	// on lance le script lua
	if (luaL_dofile(state,"script.lua")!=0)
	{
		// il y a eut une erreur dans le script
		fprintf(stderr,"%s\n",lua_tostring(state,-1));
		exit(0);
	}

	// recuperation d'une variable (number) Lua depuis le code C++
	lua_settop(state,0);
	lua_getglobal(state,"var");
	if (lua_isnumber(state,1))
	{
		printf("valeur de la variable var : %f\n",lua_tonumber(state,1));
	}
	lua_pop(state,1);

	// recuperation d'une variable (string) Lua depuis le code C++
	lua_settop(state,0);
	lua_getglobal(state,"toto");
	if (lua_isstring(state,1))
	{
		printf("valeur de la variable toto : %s\n",lua_tostring(state,1));
	}
	lua_pop(state,1);

	//recuperation du contenu d'un tableau Lua depuis le code C++
	lua_settop(state,0);
	lua_getglobal(state,"tableau");
	if (!lua_istable(state,1))
	{
		fprintf(stderr,"la variable tableau n'est pas un tableau\n");
		lua_pop(state,1);
	}
	else
	{
		// on veut adresser la variable d'indice 2
		lua_pushnumber(state,2);
		// maintnenant on a dans la pile l'indice à la place 1 et le tableau
		// à la place 2 (car on a empilé l'indice par dessus)
		// on demande à Lua de remplacer le haut de la pile (donc notre indice)
		// par la contenu de la case du tableau en lui donnant l'adresse du tableau
		lua_gettable(state,-2);
		// on verifie la donnée recuperée
		if (lua_isstring(state,-1))
		{
			printf("valeur recuperé à l'indice 2 : %s\n",lua_tostring(state,-1));
			// on depile les deux elements (la valeure de la case et le nom
			// du tableau)
			lua_pop(state,2);
		}	
	}

	// appel d'une fonction lua depuis le code C++
	// on empile le nom de la fonction qu'on souhaite lancer
	lua_getglobal(state,"luaPrint");
	// on verifie si la fonction existe bien
	if (!lua_isfunction(state,-1))
	{
		// la fonction n'existe pas
		fprintf(stderr,"la fonction luaPrint n'existe pas\n");
		lua_pop(state,1);
	}
	else
	{
		// la fonction existe, on lui passe une chaine de caractere comme
		// premier argument
		lua_pushstring(state,"le message passé en parametre\n");
		// on appel la fonction qui a 1 argument et 0 retour
		lua_call(state,1,0);
	}

	// on empile le nom de la fonction qu'on souhaite lancer
	lua_getglobal(state,"addition");
	// on verifie si la fonction existe bien
	if (!lua_isfunction(state,-1))
	{
		// la fonction n'existe pas
		fprintf(stderr,"la fonction addition n'existe pas\n");
		lua_pop(state,1);
	}
	else
	{
		// la fonction existe, on lui passe une chaine de caractere comme
		// premier argument
		lua_pushnumber(state,10);
		lua_pushnumber(state,20);
		// on appel la fonction qui a 2 argument et 1 retour
		lua_call(state,2,1);
		// on recupere la valeur de retour
		float retour = (float)lua_tonumber(state,-1);
		printf("valeur de retour : %f\n",retour);
	}

	return 0;
}
