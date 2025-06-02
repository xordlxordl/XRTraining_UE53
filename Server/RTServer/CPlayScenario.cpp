#include "MyInclude.h"


unsigned int CPlayScenario::ThreadProc()
{
    while( IsRun() )
    {
        Sleep(1);
    }
    return 0;
}

CPlayScenario::CPlayScenario()
{
    m_SCTFileName.clear();
}

CPlayScenario::~CPlayScenario()
{
    CThread::End();
}

vector<string> CPlayScenario::split( string s, string delimiter )
{
    vector<string> res;
    res.clear();

    size_t pos_start = 0, pos_end = 0, delim_len = delimiter.length();
    string token;

    while( ( pos_end = s.find( delimiter, pos_start ) ) != string::npos )
    {
        token = s.substr( pos_start, pos_end - pos_start );
        pos_start = pos_end + delim_len;
        res.push_back( token );
    }

    res.push_back( s.substr( pos_start ) );
    return res;
}

void CPlayScenario::LoadSCT()
{
    if( m_SCTFileName.length() <= 0 )
    {
        Log( "m_SCTFileName Not Loaded" );

        return;
    }

    Log( "%s SCTFileName", m_SCTFileName.c_str() );

    //"utf-8.military.sct.json"
    string filename = m_SCTFileName;
    std::ifstream file( filename, std::ios::binary );
    file.seekg( 0, std::ios::end );
    size_t size = ( size_t ) file.tellg();

    //skip BOM
    file.seekg( 2, std::ios::beg );
    size -= 2;

    std::u16string u16( ( size / 2 ) + 1, '\0' );
    file.read( ( char* ) &u16[ 0 ], size );

    file.close();

    std::string utf8 = std::wstring_convert< std::codecvt_utf8_utf16< char16_t >, char16_t >{}.to_bytes( u16 );
    string str = boost::locale::conv::from_utf<char>( utf8, "EUC-KR" );

/*
    Json::CharReaderBuilder builder;
    Json::Value root;
    Json::Reader reader;
    bool bResult = reader.parse( str, root );
    */

    JS::ParseContext context( str );

    context.parseTo( m_SctData );

    //std::string pretty_json = JS::serializeStruct( stData );
    ParseSCT();
    
}

void CPlayScenario::ParseSCT()
{
    
    int nSize = (int)m_SctData.eventList.size();
    int i;
    for( i = 0; i < nSize; ++i )
    {
        FRtScenarioEvent* pEvent = &m_SctData.eventList.at( i );

        FRtScenarioEventCommand cmd;
        cmd.EventID = pEvent->eventNameTId;
        cmd.OwnerObjectID = pEvent->ownerId;
        cmd.TargetObjectID = pEvent->targetId;

        cmd.EventDataIndex = i;

        int ConSize = (int)pEvent->eventOccurConditionList.size();

        string jsonstr;
        Json::Value root;
        
        int j;
        for( j = 0; j < ConSize; ++j )
        {
            FRtScenarioCondition* pCon = &pEvent->eventOccurConditionList.at( j );

            vector<string> str = split( pCon->target1Value, "," );

            int strsize = (int)str.size();

            string json;
            
            Json::Value itemlist;
            
            int k;
            for( k = 0; k < strsize; ++k )
            {
                Json::Value item;
                item[ "ERtSctFunCmd" ] = str.at( k );
                itemlist.append( item );
            }
            root["fnlist"].append(itemlist);
            jsonstr = root.toStyledString();
            

        }
        int ProcSize = (int)pEvent->eventProcessList.size();

        int p;
        for( p = 0; p < ProcSize; ++p )
        {
            FRtEventProcess* pProc = &pEvent->eventProcessList.at( p );

            vector<string> strProc = split( pProc->processValue, "," );
            
            int a = 0;

        }

        //

    }
    
}

void CPlayScenario::PlayStart()
{
    CThread::End();
    CThread::Begin();
}

int CPlayScenario::GetSctPlayerCount()
{


    return m_SctData.playerCount;
}

bool CPlayScenario::IsSctObjectID( char* id )
{
    if( id == NULL )
        return false;

    int size = (int)m_SctData.playerGroupList.size();

    int i;
    for( i = 0; i < size; ++i )
    {
        FRtScenarioPlayerGroup* pGroup = &m_SctData.playerGroupList.at( i );

        if( pGroup )
        {
            int gsize = (int)pGroup->playerList.size();

            int j;
            for( j = 0; j < gsize; ++j )
            {
                FRtScenarioPlayer* pPlayer = &pGroup->playerList.at( j );

                if( pPlayer )
                {
                    string pName = pPlayer->playerObjectId;

                    if( pName.compare( id ) == 0 )
                        return true;

                }
            }

        }

    }
    return false;
}

