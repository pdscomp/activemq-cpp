/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <activemq/commands/ActiveMQTempTopic.h>

#include <activemq/util/CMSExceptionSupport.h>

using namespace std;
using namespace activemq;
using namespace activemq::commands;

////////////////////////////////////////////////////////////////////////////////
ActiveMQTempTopic::ActiveMQTempTopic() : ActiveMQTempDestination() {
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQTempTopic::ActiveMQTempTopic( const std::string& name ) :
    ActiveMQTempDestination( name )
{}

////////////////////////////////////////////////////////////////////////////////
ActiveMQTempTopic::~ActiveMQTempTopic() {
    try {
        this->close();
    }
    AMQ_CATCHALL_NOTHROW()
}

////////////////////////////////////////////////////////////////////////////////
unsigned char ActiveMQTempTopic::getDataStructureType() const {
    return ActiveMQTempTopic::ID_ACTIVEMQTEMPTOPIC;
}

////////////////////////////////////////////////////////////////////////////////
ActiveMQTempTopic* ActiveMQTempTopic::cloneDataStructure() const {
    std::auto_ptr<ActiveMQTempTopic> message( new ActiveMQTempTopic() );
    message->copyDataStructure( this );
    return message.release();
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTempTopic::copyDataStructure( const DataStructure* src ) {
    ActiveMQTempDestination::copyDataStructure( src );
}

////////////////////////////////////////////////////////////////////////////////
std::string ActiveMQTempTopic::toString() const {
    return ActiveMQDestination::toString();
}

////////////////////////////////////////////////////////////////////////////////
bool ActiveMQTempTopic::equals( const DataStructure* value ) const {
    return ActiveMQDestination::equals( value );
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQTempTopic::destroy() throw ( cms::CMSException ) {
    try{
        close();
    }
    AMQ_CATCH_ALL_THROW_CMSEXCEPTION()
}