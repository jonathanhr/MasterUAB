#include <string>
#include <map>
#include <vector>
#include <assert.h>

template<class T>

class CTemplatedVectorMapManager
{
	public:
		class CMapResouceValue
		{
			public:
				CMapResouceValue(){assert(!"Llama al otro constructor");};
				T *m_Value;
				size_t m_Id;
				CMapResouceValue(T *Value, size_t Id): m_Value(Value), m_Id(Id){}
		};


		typedef std::vector<T *> TVectorResources;
		typedef std::map<std::string, CMapResouceValue> TMapResources;

	protected:
		TVectorResources m_ResourcesVector;
		TMapResources m_ResourcesMap;
	
	public:
		CTemplatedVectorMapManager();
		virtual ~CTemplatedVectorMapManager()
		{
			Destroy();		
		}

		bool RemoveResource(const std::string &Name)
		{
			int pos;
			TMapResources::iterator it = m_ResourcesMap.find(Name);
			if(it!=m_ResourcesMap.end())
			{
				pos = it->second.m_Id;
				delete m_ResourcesVector[pos];
				TVectorResources::iterator it2 = m_ResourcesVector.begin();
				std::advance(it2, pos);
				m_ResourcesVector.erase(it2);
				m_ResourcesMap.erase(it);

				for ( it = m_ResourcesMap.begin(); it != m_ResourcesMap.end(); ++it )
				{
					if(it->second.m_Id>pos)
					{
						it->second.m_Id--;
					}
				}
				return true;
			}

			return false;
		}
		
		virtual T * GetResourceById(size_t Id){
			if (Id < m_ResourcesVector.size()-1)
				return m_ResourcesVector[Id];
			return nullptr;
		}
		
		virtual T * GetResource(const std::string &Name){
			TMapResources::iterator it = m_ResourcesMap.find(Name);
			if(it!=m_ResourcesMap.end())
			{
				return it->second.m_Value;
			}
			return nullptr;
		}
		
		virtual bool AddResource(const std::string &Name, T *Resource);
		
		virtual void Destroy(){
			for (int i = 0; i < m_ResourcesVector.size(); ++i)
				delete m_ResourcesVector[i];

			m_ResourcesVector.clear();
			m_ResourcesMap.clear();
		}

		TMapResources & GetResourcesMap()
		{
			return m_ResourcesMap;
		}

		TVectorResources & GetResourcesVector()
		{
			return m_ResourcesVector;
		}
};
