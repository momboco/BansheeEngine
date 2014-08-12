#include "BsScriptSerializableList.h"
#include "BsScriptMeta.h"
#include "BsMonoField.h"
#include "BsMonoClass.h"
#include "BsMonoManager.h"
#include "BsRuntimeScriptObjects.h"
#include "BsManagedSerializableObjectInfo.h"
#include "BsScriptSerializableProperty.h"

namespace BansheeEngine
{
	ScriptSerializableList::ScriptSerializableList(MonoObject* instance, const ManagedSerializableTypeInfoListPtr& typeInfo)
		:ScriptObject(instance), mTypeInfo(typeInfo)
	{

	}

	void ScriptSerializableList::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_CreateProperty", &ScriptSerializableList::internal_createProperty);
	}

	ScriptSerializableList* ScriptSerializableList::create(const ManagedSerializableTypeInfoListPtr& typeInfo, MonoObject* object)
	{
		MonoType* monoInternalElementType = mono_class_get_type(typeInfo->mElementType->getMonoClass());
		MonoReflectionType* internalElementType = mono_type_get_object(MonoManager::instance().getDomain(), monoInternalElementType);

		void* params[2] = { object, internalElementType };
		MonoObject* managedInstance = metaData.scriptClass->createInstance(params, 2);

		ScriptSerializableList* nativeInstance = new (bs_alloc<ScriptSerializableList>()) ScriptSerializableList(managedInstance, typeInfo);

		return nativeInstance;
	}

	MonoObject* ScriptSerializableList::internal_createProperty(ScriptSerializableList* nativeInstance)
	{
		ScriptSerializableProperty* newProperty = ScriptSerializableProperty::create(nativeInstance->mTypeInfo->mElementType);

		return newProperty->getManagedInstance();
	}
}