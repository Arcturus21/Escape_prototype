#include <iostream>
#include <lua/lua.hpp>
#include <lua/lunar.h>
#include <string>
#include <sstream>
#include <math.h>

using namespace std;

class Vector3
{
private:
	friend class Lunar<Vector3>;
	static const char className[];
	static Lunar<Vector3>::RegType methods[];
	float x,y,z;
public:
	Vector3()
	{
		x=y=z=0;
	}
	Vector3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	Vector3(lua_State* L)
	{
		int nbArgs = lua_gettop(L);
		if (nbArgs == 3 && lua_isnumber(L,1) &&
			lua_isnumber(L,2) && lua_isnumber(L,3))
		{
			this->x = lua_tonumber(L,1);
			this->y = lua_tonumber(L,2);
			this->z = lua_tonumber(L,3);
		}
		else if (nbArgs == 0)
		{
			x = y = z = 0;
		}
		else
		{
			cerr << "Vector3 : mauvais parametres" << endl;
		}
	}

	void set(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	int set(lua_State* L)
	{
		int nbArgs = lua_gettop(L);
		if (nbArgs == 3 && lua_isnumber(L,1) &&
			lua_isnumber(L,2) && lua_isnumber(L,3))
		{
			this->x = lua_tonumber(L,1);
			this->y = lua_tonumber(L,2);
			this->z = lua_tonumber(L,3);
		}
		else if (nbArgs == 0)
		{
			x = y = z = 0;
		}
		else
		{
			cerr << "Vector3:set() : mauvais parametres" << endl;
		}
		return 0;
	}
	int print(lua_State* L)
	{
		cout << "{" << x << "," << y << "," << z << "}" << endl;
		return 0;
	}
	float length()
	{
		return sqrt(x*x + y*y + z*z);
	}
	int length(lua_State* L)
	{
		lua_pushnumber(L,length());
		return 1;
	}
	int add(lua_State* L)
	{
		Vector3* param = Lunar<Vector3>::check(L,1);
		if (param == NULL)
		{
			cerr << "Vector3:add() : mauvais type de paramètre" << endl;
			return 0;
		}
		Vector3* ret = new Vector3(x+param->x,y+param->y,z+param->z);
		// on souhaite que l'objet soit géré par le garbage collector
		Lunar<Vector3>::push(L,ret,true);
		return 1;
	}
	int equal(lua_State* L)
	{
		Vector3* param = Lunar<Vector3>::check(L,1);
		if (param == NULL)
		{
			cerr << "Vector3:equal() : mauvais type de paramètre" << endl;
			return 0;
		}
		bool ret = x==param->x && y==param->y && z==param->z;
		lua_pushboolean(L,ret);
		return 1;
	}
	string toString()
	{
		std::stringstream s;
		s << "{" << x << "," << y << "," << z << "}";
		return s.str();
	}
};

std::ostream& operator<<(std::ostream& stream, Vector3& vector)
{
	return stream << vector.toString();
}
const char Vector3::className[] = "Vector3";

#define method(class, name) {#name, &class::name}

Lunar<Vector3>::RegType Vector3::methods[] = {
  method(Vector3, set),
  method(Vector3, print),
  method(Vector3, length),
  method(Vector3, equal),
  {"add",&Vector3::add}, // une autre facon d'enregistrer une methode
  {0,0}
};

int main(int argc, char** argv)
{
	lua_State * state;
	// on crée un context d'execution de Lua
	state = lua_open();
	// on charge les librairies standards de Lua
	luaL_openlibs(state);

	// on enregistre la classe aupres de lua
	Lunar<Vector3>::Register(state);

	// on lance le script lua
	if (luaL_dofile(state,"script.lua")!=0)
	{
		// il y a eut une erreur dans le script
		fprintf(stderr,"%s\n",lua_tostring(state,-1));
		exit(0);
	}

	// appel d'une methode 
	lua_settop(state,0);
	Vector3 vect(12,13,14);
	Lunar<Vector3>::push(state,&vect);
	Lunar<Vector3>::call(state,"printLength",0,0);

	// appel d'une methode avec parametres
	lua_settop(state,0);
	Lunar<Vector3>::push(state,&vect);
	lua_pushnumber(state,1);
	lua_pushnumber(state,1);
	lua_pushnumber(state,1);
	Lunar<Vector3>::call(state,"set",3,0);
	// on affiche le resultat
	cout << vect << endl;

	// appel d'une methode avec parametres
	lua_settop(state,0);
	Vector3 v1(10,10,10);
	Vector3 v2(10,10,10);
	// on empile le vecteur à appeler
	Lunar<Vector3>::push(state,&v1);
	// on empile le parametre
	Lunar<Vector3>::push(state,&v2);
	Lunar<Vector3>::call(state,"equal",1,1);
	if (lua_isboolean(state,-1))
	{
		bool ret = lua_toboolean(state,-1);
		if (ret)
		{
			cout << "les deux vecteur sont égaux" << endl;
		}
		else
		{
			cout << "les deux vecteur ne sont pas égaux" << endl;
		}
	}
	else
	{
		cerr << "probleme lors de l'appel à vect.length()" << endl;
	}

	// appel d'une methode avec retour
	lua_settop(state,0);
	Lunar<Vector3>::push(state,&vect);
	Lunar<Vector3>::call(state,"length",0,1);
	if (lua_isnumber(state,-1))
	{
		float ret = lua_tonumber(state,-1);
		cout << "resultat de l'appel à vect.length() : " << ret << endl;
		if (ret == vect.length())
		{
			cout << "Lua à bien retourné le même resultat que le C++" << endl;
		}
		else
		{
			cerr << "Lua n'a pas retourné le même résultat que le C++" << endl;
		}
	}
	else
	{
		cerr << "probleme lors de l'appel à vect.length()" << endl;
	}

	// appel d'une methode avec objet en retour
	lua_settop(state,0);
	Vector3 add1(10,10,10);
	Vector3 add2(20,20,20);
	// on empile le vecteur à appeler
	Lunar<Vector3>::push(state,&add1);
	// on empile le parametre
	Lunar<Vector3>::push(state,&add2);
	Lunar<Vector3>::call(state,"add",1,1);
	// on a maintenant le resultat en sommet de pile.
	//on verifie qu'il est du bon type
	cout << lua_gettop(state) << endl;
	Vector3* ret = Lunar<Vector3>::check(state,1);
	if (ret == NULL)
	{
		cerr << "erreur de type au retour de l'addition" << endl;
	}
	else
	{
		// on affiche le resultat
		cout << "retour de l'addition :" << ret->toString() << endl;
	}

	lua_close(state);
	return 0;
}
