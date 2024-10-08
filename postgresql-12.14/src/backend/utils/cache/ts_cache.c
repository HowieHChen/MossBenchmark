                                                                            
   
              
                                    
   
                                                                    
                                                                 
                
   
                                                                            
                                                                       
                                                                          
                                                                          
                                                                          
                                                                          
                                                                         
                        
   
   
                                                                
   
                  
                                        
   
                                                                            
   
#include "postgres.h"

#include "access/genam.h"
#include "access/htup_details.h"
#include "access/table.h"
#include "access/xact.h"
#include "catalog/indexing.h"
#include "catalog/namespace.h"
#include "catalog/pg_ts_config.h"
#include "catalog/pg_ts_config_map.h"
#include "catalog/pg_ts_dict.h"
#include "catalog/pg_ts_parser.h"
#include "catalog/pg_ts_template.h"
#include "commands/defrem.h"
#include "miscadmin.h"
#include "tsearch/ts_cache.h"
#include "utils/builtins.h"
#include "utils/catcache.h"
#include "utils/fmgroids.h"
#include "utils/inval.h"
#include "utils/lsyscache.h"
#include "utils/memutils.h"
#include "utils/regproc.h"
#include "utils/syscache.h"

   
                                                                         
                                                                        
                                                                          
                            
   
#define MAXTOKENTYPE 256
#define MAXDICTSPERTT 100

static HTAB *TSParserCacheHash = NULL;
static TSParserCacheEntry *lastUsedParser = NULL;

static HTAB *TSDictionaryCacheHash = NULL;
static TSDictionaryCacheEntry *lastUsedDictionary = NULL;

static HTAB *TSConfigCacheHash = NULL;
static TSConfigCacheEntry *lastUsedConfig = NULL;

   
                                                                           
   
char *TSCurrentConfig = NULL;

static Oid TSCurrentConfigCache = InvalidOid;

   
                                                                         
                                                                          
   
                                                                           
                                                                       
                                                                           
                                      
   
                                                                      
                               
   
static void
InvalidateTSCacheCallBack(Datum arg, int cacheid, uint32 hashvalue)
{
  HTAB *hash = (HTAB *)DatumGetPointer(arg);
  HASH_SEQ_STATUS status;
  TSAnyCacheEntry *entry;

  hash_seq_init(&status, hash);
  while ((entry = (TSAnyCacheEntry *)hash_seq_search(&status)) != NULL)
  {
    entry->isvalid = false;
  }

                                                                     
  if (hash == TSConfigCacheHash)
  {
    TSCurrentConfigCache = InvalidOid;
  }
}

   
                            
   
TSParserCacheEntry *
lookup_ts_parser_cache(Oid prsId)
{
  TSParserCacheEntry *entry;

  if (TSParserCacheHash == NULL)
  {
                                                       
    HASHCTL ctl;

    MemSet(&ctl, 0, sizeof(ctl));
    ctl.keysize = sizeof(Oid);
    ctl.entrysize = sizeof(TSParserCacheEntry);
    TSParserCacheHash = hash_create("Tsearch parser cache", 4, &ctl, HASH_ELEM | HASH_BLOBS);
                                             
    CacheRegisterSyscacheCallback(TSPARSEROID, InvalidateTSCacheCallBack, PointerGetDatum(TSParserCacheHash));

                                                  
    if (!CacheMemoryContext)
    {
      CreateCacheMemoryContext();
    }
  }

                                
  if (lastUsedParser && lastUsedParser->prsId == prsId && lastUsedParser->isvalid)
  {
    return lastUsedParser;
  }

                                        
  entry = (TSParserCacheEntry *)hash_search(TSParserCacheHash, (void *)&prsId, HASH_FIND, NULL);
  if (entry == NULL || !entry->isvalid)
  {
       
                                                                         
                                          
       
    HeapTuple tp;
    Form_pg_ts_parser prs;

    tp = SearchSysCache1(TSPARSEROID, ObjectIdGetDatum(prsId));
    if (!HeapTupleIsValid(tp))
    {
      elog(ERROR, "cache lookup failed for text search parser %u", prsId);
    }
    prs = (Form_pg_ts_parser)GETSTRUCT(tp);

       
                     
       
    if (!OidIsValid(prs->prsstart))
    {
      elog(ERROR, "text search parser %u has no prsstart method", prsId);
    }
    if (!OidIsValid(prs->prstoken))
    {
      elog(ERROR, "text search parser %u has no prstoken method", prsId);
    }
    if (!OidIsValid(prs->prsend))
    {
      elog(ERROR, "text search parser %u has no prsend method", prsId);
    }

    if (entry == NULL)
    {
      bool found;

                                    
      entry = (TSParserCacheEntry *)hash_search(TSParserCacheHash, (void *)&prsId, HASH_ENTER, &found);
      Assert(!found);                                   
    }

    MemSet(entry, 0, sizeof(TSParserCacheEntry));
    entry->prsId = prsId;
    entry->startOid = prs->prsstart;
    entry->tokenOid = prs->prstoken;
    entry->endOid = prs->prsend;
    entry->headlineOid = prs->prsheadline;
    entry->lextypeOid = prs->prslextype;

    ReleaseSysCache(tp);

    fmgr_info_cxt(entry->startOid, &entry->prsstart, CacheMemoryContext);
    fmgr_info_cxt(entry->tokenOid, &entry->prstoken, CacheMemoryContext);
    fmgr_info_cxt(entry->endOid, &entry->prsend, CacheMemoryContext);
    if (OidIsValid(entry->headlineOid))
    {
      fmgr_info_cxt(entry->headlineOid, &entry->prsheadline, CacheMemoryContext);
    }

    entry->isvalid = true;
  }

  lastUsedParser = entry;

  return entry;
}

   
                                
   
TSDictionaryCacheEntry *
lookup_ts_dictionary_cache(Oid dictId)
{
  TSDictionaryCacheEntry *entry;

  if (TSDictionaryCacheHash == NULL)
  {
                                                       
    HASHCTL ctl;

    MemSet(&ctl, 0, sizeof(ctl));
    ctl.keysize = sizeof(Oid);
    ctl.entrysize = sizeof(TSDictionaryCacheEntry);
    TSDictionaryCacheHash = hash_create("Tsearch dictionary cache", 8, &ctl, HASH_ELEM | HASH_BLOBS);
                                                              
    CacheRegisterSyscacheCallback(TSDICTOID, InvalidateTSCacheCallBack, PointerGetDatum(TSDictionaryCacheHash));
    CacheRegisterSyscacheCallback(TSTEMPLATEOID, InvalidateTSCacheCallBack, PointerGetDatum(TSDictionaryCacheHash));

                                                  
    if (!CacheMemoryContext)
    {
      CreateCacheMemoryContext();
    }
  }

                                
  if (lastUsedDictionary && lastUsedDictionary->dictId == dictId && lastUsedDictionary->isvalid)
  {
    return lastUsedDictionary;
  }

                                        
  entry = (TSDictionaryCacheEntry *)hash_search(TSDictionaryCacheHash, (void *)&dictId, HASH_FIND, NULL);
  if (entry == NULL || !entry->isvalid)
  {
       
                                                                         
                                          
       
    HeapTuple tpdict, tptmpl;
    Form_pg_ts_dict dict;
    Form_pg_ts_template template;
    MemoryContext saveCtx;

    tpdict = SearchSysCache1(TSDICTOID, ObjectIdGetDatum(dictId));
    if (!HeapTupleIsValid(tpdict))
    {
      elog(ERROR, "cache lookup failed for text search dictionary %u", dictId);
    }
    dict = (Form_pg_ts_dict)GETSTRUCT(tpdict);

       
                     
       
    if (!OidIsValid(dict->dicttemplate))
    {
      elog(ERROR, "text search dictionary %u has no template", dictId);
    }

       
                                      
       
    tptmpl = SearchSysCache1(TSTEMPLATEOID, ObjectIdGetDatum(dict->dicttemplate));
    if (!HeapTupleIsValid(tptmpl))
    {
      elog(ERROR, "cache lookup failed for text search template %u", dict->dicttemplate);
    }
    template = (Form_pg_ts_template)GETSTRUCT(tptmpl);

       
                     
       
    if (!OidIsValid(template->tmpllexize))
    {
      elog(ERROR, "text search template %u has no lexize method", template->tmpllexize);
    }

    if (entry == NULL)
    {
      bool found;

                                    
      entry = (TSDictionaryCacheEntry *)hash_search(TSDictionaryCacheHash, (void *)&dictId, HASH_ENTER, &found);
      Assert(!found);                                   

                                                                
      saveCtx = AllocSetContextCreate(CacheMemoryContext, "TS dictionary", ALLOCSET_SMALL_SIZES);
      MemoryContextCopyAndSetIdentifier(saveCtx, NameStr(dict->dictname));
    }
    else
    {
                                                      
      saveCtx = entry->dictCtx;
                                                                      
      MemoryContextSetIdentifier(saveCtx, NULL);
      MemoryContextReset(saveCtx);
      MemoryContextCopyAndSetIdentifier(saveCtx, NameStr(dict->dictname));
    }

    MemSet(entry, 0, sizeof(TSDictionaryCacheEntry));
    entry->dictId = dictId;
    entry->dictCtx = saveCtx;

    entry->lexizeOid = template->tmpllexize;

    if (OidIsValid(template->tmplinit))
    {
      List *dictoptions;
      Datum opt;
      bool isnull;
      MemoryContext oldcontext;

         
                                                                         
                                                    
         
      oldcontext = MemoryContextSwitchTo(entry->dictCtx);

      opt = SysCacheGetAttr(TSDICTOID, tpdict, Anum_pg_ts_dict_dictinitoption, &isnull);
      if (isnull)
      {
        dictoptions = NIL;
      }
      else
      {
        dictoptions = deserialize_deflist(opt);
      }

      entry->dictData = DatumGetPointer(OidFunctionCall1(template->tmplinit, PointerGetDatum(dictoptions)));

      MemoryContextSwitchTo(oldcontext);
    }

    ReleaseSysCache(tptmpl);
    ReleaseSysCache(tpdict);

    fmgr_info_cxt(entry->lexizeOid, &entry->lexize, entry->dictCtx);

    entry->isvalid = true;
  }

  lastUsedDictionary = entry;

  return entry;
}

   
                                                                        
                                                                          
                                      
   
static void
init_ts_config_cache(void)
{
  HASHCTL ctl;

  MemSet(&ctl, 0, sizeof(ctl));
  ctl.keysize = sizeof(Oid);
  ctl.entrysize = sizeof(TSConfigCacheEntry);
  TSConfigCacheHash = hash_create("Tsearch configuration cache", 16, &ctl, HASH_ELEM | HASH_BLOBS);
                                                                
  CacheRegisterSyscacheCallback(TSCONFIGOID, InvalidateTSCacheCallBack, PointerGetDatum(TSConfigCacheHash));
  CacheRegisterSyscacheCallback(TSCONFIGMAP, InvalidateTSCacheCallBack, PointerGetDatum(TSConfigCacheHash));

                                                
  if (!CacheMemoryContext)
  {
    CreateCacheMemoryContext();
  }
}

   
                                   
   
TSConfigCacheEntry *
lookup_ts_config_cache(Oid cfgId)
{
  TSConfigCacheEntry *entry;

  if (TSConfigCacheHash == NULL)
  {
                                                       
    init_ts_config_cache();
  }

                                
  if (lastUsedConfig && lastUsedConfig->cfgId == cfgId && lastUsedConfig->isvalid)
  {
    return lastUsedConfig;
  }

                                        
  entry = (TSConfigCacheEntry *)hash_search(TSConfigCacheHash, (void *)&cfgId, HASH_FIND, NULL);
  if (entry == NULL || !entry->isvalid)
  {
       
                                                                         
                                          
       
    HeapTuple tp;
    Form_pg_ts_config cfg;
    Relation maprel;
    Relation mapidx;
    ScanKeyData mapskey;
    SysScanDesc mapscan;
    HeapTuple maptup;
    ListDictionary maplists[MAXTOKENTYPE + 1];
    Oid mapdicts[MAXDICTSPERTT];
    int maxtokentype;
    int ndicts;
    int i;

    tp = SearchSysCache1(TSCONFIGOID, ObjectIdGetDatum(cfgId));
    if (!HeapTupleIsValid(tp))
    {
      elog(ERROR, "cache lookup failed for text search configuration %u", cfgId);
    }
    cfg = (Form_pg_ts_config)GETSTRUCT(tp);

       
                     
       
    if (!OidIsValid(cfg->cfgparser))
    {
      elog(ERROR, "text search configuration %u has no parser", cfgId);
    }

    if (entry == NULL)
    {
      bool found;

                                    
      entry = (TSConfigCacheEntry *)hash_search(TSConfigCacheHash, (void *)&cfgId, HASH_ENTER, &found);
      Assert(!found);                                   
    }
    else
    {
                                
      if (entry->map)
      {
        for (i = 0; i < entry->lenmap; i++)
        {
          if (entry->map[i].dictIds)
          {
            pfree(entry->map[i].dictIds);
          }
        }
        pfree(entry->map);
      }
    }

    MemSet(entry, 0, sizeof(TSConfigCacheEntry));
    entry->cfgId = cfgId;
    entry->prsId = cfg->cfgparser;

    ReleaseSysCache(tp);

       
                                                                           
       
                                                                         
                                                                        
                                                                       
       
    MemSet(maplists, 0, sizeof(maplists));
    maxtokentype = 0;
    ndicts = 0;

    ScanKeyInit(&mapskey, Anum_pg_ts_config_map_mapcfg, BTEqualStrategyNumber, F_OIDEQ, ObjectIdGetDatum(cfgId));

    maprel = table_open(TSConfigMapRelationId, AccessShareLock);
    mapidx = index_open(TSConfigMapIndexId, AccessShareLock);
    mapscan = systable_beginscan_ordered(maprel, mapidx, NULL, 1, &mapskey);

    while ((maptup = systable_getnext_ordered(mapscan, ForwardScanDirection)) != NULL)
    {
      Form_pg_ts_config_map cfgmap = (Form_pg_ts_config_map)GETSTRUCT(maptup);
      int toktype = cfgmap->maptokentype;

      if (toktype <= 0 || toktype > MAXTOKENTYPE)
      {
        elog(ERROR, "maptokentype value %d is out of range", toktype);
      }
      if (toktype < maxtokentype)
      {
        elog(ERROR, "maptokentype entries are out of order");
      }
      if (toktype > maxtokentype)
      {
                                                                      
        if (ndicts > 0)
        {
          maplists[maxtokentype].len = ndicts;
          maplists[maxtokentype].dictIds = (Oid *)MemoryContextAlloc(CacheMemoryContext, sizeof(Oid) * ndicts);
          memcpy(maplists[maxtokentype].dictIds, mapdicts, sizeof(Oid) * ndicts);
        }
        maxtokentype = toktype;
        mapdicts[0] = cfgmap->mapdict;
        ndicts = 1;
      }
      else
      {
                                                    
        if (ndicts >= MAXDICTSPERTT)
        {
          elog(ERROR, "too many pg_ts_config_map entries for one token type");
        }
        mapdicts[ndicts++] = cfgmap->mapdict;
      }
    }

    systable_endscan_ordered(mapscan);
    index_close(mapidx, AccessShareLock);
    table_close(maprel, AccessShareLock);

    if (ndicts > 0)
    {
                                                   
      maplists[maxtokentype].len = ndicts;
      maplists[maxtokentype].dictIds = (Oid *)MemoryContextAlloc(CacheMemoryContext, sizeof(Oid) * ndicts);
      memcpy(maplists[maxtokentype].dictIds, mapdicts, sizeof(Oid) * ndicts);
                                    
      entry->lenmap = maxtokentype + 1;
      entry->map = (ListDictionary *)MemoryContextAlloc(CacheMemoryContext, sizeof(ListDictionary) * entry->lenmap);
      memcpy(entry->map, maplists, sizeof(ListDictionary) * entry->lenmap);
    }

    entry->isvalid = true;
  }

  lastUsedConfig = entry;

  return entry;
}

                                                      
                                             
                                                      
   

Oid
getTSCurrentConfig(bool emitError)
{
                                            
  if (OidIsValid(TSCurrentConfigCache))
  {
    return TSCurrentConfigCache;
  }

                                          
  if (TSCurrentConfig == NULL || *TSCurrentConfig == '\0')
  {
    if (emitError)
    {
      elog(ERROR, "text search configuration isn't set");
    }
    else
    {
      return InvalidOid;
    }
  }

  if (TSConfigCacheHash == NULL)
  {
                                                                    
    init_ts_config_cache();
  }

                          
  TSCurrentConfigCache = get_ts_config_oid(stringToQualifiedNameList(TSCurrentConfig), !emitError);

  return TSCurrentConfigCache;
}

                                                   
bool
check_TSCurrentConfig(char **newval, void **extra, GucSource source)
{
     
                                                                       
                                                                         
                              
     
  if (IsTransactionState() && MyDatabaseId != InvalidOid)
  {
    Oid cfgId;
    HeapTuple tuple;
    Form_pg_ts_config cfg;
    char *buf;

    cfgId = get_ts_config_oid(stringToQualifiedNameList(*newval), true);

       
                                                                 
                                                                         
       
    if (!OidIsValid(cfgId))
    {
      if (source == PGC_S_TEST)
      {
        ereport(NOTICE, (errcode(ERRCODE_UNDEFINED_OBJECT), errmsg("text search configuration \"%s\" does not exist", *newval)));
        return true;
      }
      else
      {
        return false;
      }
    }

       
                                                                         
                                                     
       
    tuple = SearchSysCache1(TSCONFIGOID, ObjectIdGetDatum(cfgId));
    if (!HeapTupleIsValid(tuple))
    {
      elog(ERROR, "cache lookup failed for text search configuration %u", cfgId);
    }
    cfg = (Form_pg_ts_config)GETSTRUCT(tuple);

    buf = quote_qualified_identifier(get_namespace_name(cfg->cfgnamespace), NameStr(cfg->cfgname));

    ReleaseSysCache(tuple);

                                            
    free(*newval);
    *newval = strdup(buf);
    pfree(buf);
    if (!*newval)
    {
      return false;
    }
  }

  return true;
}

                                                    
void
assign_TSCurrentConfig(const char *newval, void *extra)
{
                                                           
  TSCurrentConfigCache = InvalidOid;
}
