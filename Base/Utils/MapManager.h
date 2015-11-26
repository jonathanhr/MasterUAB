#ifndef _MAP_MANAGER
#define _MAP_MANAGER

#pragma once

#include <stdlib.h>
#include <map>
#include <string>
#include <stdio.h>

template<class T>


class CMapManager
{
	protected:
		typedef std::map<std::string, T*> TMapResource;
		TMapResource m_Resources;

	public:
		CMapManager(){}

		virtual ~CMapManager()
		{
			Destroy();
		}

		virtual T * GetResource(const std::string &Name)
		{
			TMapResource::iterator it = m_Resources.find(Name);
			if(it!=m_Resources.end())
			{
				return it->second;
			}
			return nullptr;
		}

		virtual bool AddResource(const std::string &Name, T *Resource)
		{
			T* aux = GetResource(Name);
			if(aux==nullptr)
			{
				m_Resources.insert(std::pair<std::string,T*>(Name,Resource));
				return true;
			}
			return false;
		}


		void Destroy()
		{
			TMapResource::iterator it;
			for ( it = m_Resources.begin(); it != m_Resources.end(); ++it )
			{
				delete it->second;
			}
			m_Resources.clear();
		}
};

#endif