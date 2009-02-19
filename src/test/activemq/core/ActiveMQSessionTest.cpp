/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ActiveMQSessionTest.h"

#include <decaf/lang/Thread.h>
#include <activemq/transport/mock/MockTransportFactory.h>
#include <activemq/transport/TransportRegistry.h>
#include <decaf/net/Socket.h>
#include <decaf/net/ServerSocket.h>
#include <activemq/commands/ActiveMQTextMessage.h>
#include <activemq/commands/ConsumerId.h>
#include <activemq/commands/MessageDispatch.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/core/ActiveMQSession.h>
#include <activemq/core/ActiveMQConsumer.h>
#include <activemq/core/ActiveMQProducer.h>
#include <decaf/util/Properties.h>
#include <decaf/util/Date.h>
#include <decaf/lang/Pointer.h>

using namespace std;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::commands;
using namespace decaf;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionTest::testAutoAcking() {

    MyCMSMessageListener msgListener1;
    MyCMSMessageListener msgListener2;

    CPPUNIT_ASSERT( connection != NULL );

    // Create an Auto Ack Session
    cms::Session* session = connection->createSession();

    // Create a Topic
    cms::Topic* topic1 = session->createTopic( "TestTopic1" );
    cms::Topic* topic2 = session->createTopic( "TestTopic2" );

    CPPUNIT_ASSERT( topic1 != NULL );
    CPPUNIT_ASSERT( topic2 != NULL );

    // Create a consumer
    ActiveMQConsumer* consumer1 =
        dynamic_cast<ActiveMQConsumer*>( session->createConsumer( topic1 ) );
    ActiveMQConsumer* consumer2 =
        dynamic_cast<ActiveMQConsumer*>( session->createConsumer( topic2 ) );

    CPPUNIT_ASSERT( consumer1 != NULL );
    CPPUNIT_ASSERT( consumer2 != NULL );

    CPPUNIT_ASSERT( consumer1->getMessageSelector() == "" );
    CPPUNIT_ASSERT( consumer2->getMessageSelector() == "" );

    CPPUNIT_ASSERT( consumer1->receiveNoWait() == NULL );
    CPPUNIT_ASSERT( consumer1->receive( 5 ) == NULL );
    CPPUNIT_ASSERT( consumer2->receiveNoWait() == NULL );
    CPPUNIT_ASSERT( consumer2->receive( 5 ) == NULL );

    consumer1->setMessageListener( &msgListener1 );
    consumer2->setMessageListener( &msgListener2 );

    injectTextMessage( "This is a Test 1" , *topic1, consumer1->getConsumerId() );

    synchronized( &msgListener1.mutex )
    {
        if( msgListener1.messages.size() == 0 )
        {
            msgListener1.mutex.wait( 3000 );
        }
    }

    CPPUNIT_ASSERT( msgListener1.messages.size() == 1 );

    injectTextMessage( "This is a Test 2" , *topic2, consumer2->getConsumerId() );

    synchronized( &msgListener2.mutex )
    {
        if( msgListener2.messages.size() == 0 )
        {
            msgListener2.mutex.wait( 3000 );
        }
    }

    CPPUNIT_ASSERT( msgListener2.messages.size() == 1 );

    cms::TextMessage* msg1 =
        dynamic_cast< cms::TextMessage* >(
            msgListener1.messages[0] );
    cms::TextMessage* msg2 =
        dynamic_cast< cms::TextMessage* >(
            msgListener2.messages[0] );

    CPPUNIT_ASSERT( msg1 != NULL );
    CPPUNIT_ASSERT( msg2 != NULL );

    std::string text1 = msg1->getText();
    std::string text2 = msg2->getText();

    CPPUNIT_ASSERT( text1 == "This is a Test 1" );
    CPPUNIT_ASSERT( text2 == "This is a Test 2" );

    delete topic1;
    delete topic2;

    delete consumer1;
    delete consumer2;

    delete session;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionTest::testClientAck()
{
    MyCMSMessageListener msgListener1( true );
    MyCMSMessageListener msgListener2( true );

    CPPUNIT_ASSERT( connection != NULL );

    // Create an Auto Ack Session
    cms::Session* session = connection->createSession(
        cms::Session::CLIENT_ACKNOWLEDGE );

    // Create a Topic
    cms::Topic* topic1 = session->createTopic( "TestTopic1");
    cms::Topic* topic2 = session->createTopic( "TestTopic2");

    CPPUNIT_ASSERT( topic1 != NULL );
    CPPUNIT_ASSERT( topic2 != NULL );

    // Create a consumer
    ActiveMQConsumer* consumer1 =
        dynamic_cast<ActiveMQConsumer*>( session->createConsumer( topic1 ) );
    ActiveMQConsumer* consumer2 =
        dynamic_cast<ActiveMQConsumer*>( session->createConsumer( topic2 ) );

    CPPUNIT_ASSERT( consumer1 != NULL );
    CPPUNIT_ASSERT( consumer2 != NULL );

    CPPUNIT_ASSERT( consumer1->getMessageSelector() == "" );
    CPPUNIT_ASSERT( consumer2->getMessageSelector() == "" );

    CPPUNIT_ASSERT( consumer1->receiveNoWait() == NULL );
    CPPUNIT_ASSERT( consumer1->receive( 5 ) == NULL );
    CPPUNIT_ASSERT( consumer2->receiveNoWait() == NULL );
    CPPUNIT_ASSERT( consumer2->receive( 5 ) == NULL );

    consumer1->setMessageListener( &msgListener1 );
    consumer2->setMessageListener( &msgListener2 );

    injectTextMessage( "This is a Test 1" , *topic1, consumer1->getConsumerId() );

    synchronized( &msgListener1.mutex )
    {
        if( msgListener1.messages.size() == 0 )
        {
            msgListener1.mutex.wait( 3000 );
        }
    }

    CPPUNIT_ASSERT_EQUAL( 1, (int)msgListener1.messages.size() );

    msgListener1.messages[0]->acknowledge();

    injectTextMessage( "This is a Test 2" , *topic2, consumer2->getConsumerId() );

    synchronized( &msgListener2.mutex )
    {
        if( msgListener2.messages.size() == 0 )
        {
            msgListener2.mutex.wait( 3000 );
        }
    }

    CPPUNIT_ASSERT_EQUAL( 1, (int)msgListener2.messages.size() );

    msgListener2.messages[0]->acknowledge();

    cms::TextMessage* msg1 =
        dynamic_cast< cms::TextMessage* >(
            msgListener1.messages[0] );
    cms::TextMessage* msg2 =
        dynamic_cast< cms::TextMessage* >(
            msgListener2.messages[0] );

    CPPUNIT_ASSERT( msg1 != NULL );
    CPPUNIT_ASSERT( msg2 != NULL );

    std::string text1 = msg1->getText();
    std::string text2 = msg2->getText();

    CPPUNIT_ASSERT( text1 == "This is a Test 1" );
    CPPUNIT_ASSERT( text2 == "This is a Test 2" );

    delete topic1;
    delete topic2;

    delete consumer1;
    delete consumer2;

    delete session;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionTest::testTransactional()
{
    MyCMSMessageListener msgListener1;
    MyCMSMessageListener msgListener2;

    CPPUNIT_ASSERT( connection != NULL );

    // Create an Auto Ack Session
    cms::Session* session = connection->createSession(
        cms::Session::SESSION_TRANSACTED );

    // Create a Topic
    cms::Topic* topic1 = session->createTopic( "TestTopic1");
    cms::Topic* topic2 = session->createTopic( "TestTopic2");

    CPPUNIT_ASSERT( topic1 != NULL );
    CPPUNIT_ASSERT( topic2 != NULL );

    // Create a consumer
    ActiveMQConsumer* consumer1 =
        dynamic_cast<ActiveMQConsumer*>( session->createConsumer( topic1 ) );
    ActiveMQConsumer* consumer2 =
        dynamic_cast<ActiveMQConsumer*>( session->createConsumer( topic2 ) );

    CPPUNIT_ASSERT( consumer1 != NULL );
    CPPUNIT_ASSERT( consumer2 != NULL );

    CPPUNIT_ASSERT( consumer1->getMessageSelector() == "" );
    CPPUNIT_ASSERT( consumer2->getMessageSelector() == "" );

    CPPUNIT_ASSERT( consumer1->receiveNoWait() == NULL );
    CPPUNIT_ASSERT( consumer1->receive( 5 ) == NULL );
    CPPUNIT_ASSERT( consumer2->receiveNoWait() == NULL );
    CPPUNIT_ASSERT( consumer2->receive( 5 ) == NULL );

    consumer1->setMessageListener( &msgListener1 );
    consumer2->setMessageListener( &msgListener2 );

    injectTextMessage( "This is a Test 1" , *topic1, consumer1->getConsumerId() );

    synchronized( &msgListener1.mutex )
    {
        if( msgListener1.messages.size() == 0 )
        {
            msgListener1.mutex.wait( 3000 );
        }
    }

    CPPUNIT_ASSERT_EQUAL( 1, (int)msgListener1.messages.size() );

    session->commit();

    injectTextMessage( "This is a Test 2" , *topic2, consumer2->getConsumerId() );

    synchronized( &msgListener2.mutex )
    {
        if( msgListener2.messages.size() == 0 )
        {
            msgListener2.mutex.wait( 3000 );
        }
    }

    CPPUNIT_ASSERT( msgListener2.messages.size() == 1 );

    session->commit();

    cms::TextMessage* msg1 =
        dynamic_cast< cms::TextMessage* >(
            msgListener1.messages[0] );
    cms::TextMessage* msg2 =
        dynamic_cast< cms::TextMessage* >(
            msgListener2.messages[0] );

    CPPUNIT_ASSERT( msg1 != NULL );
    CPPUNIT_ASSERT( msg2 != NULL );

    std::string text1 = msg1->getText();
    std::string text2 = msg2->getText();

    CPPUNIT_ASSERT( text1 == "This is a Test 1" );
    CPPUNIT_ASSERT( text2 == "This is a Test 2" );

    msgListener1.clear();
    msgListener2.clear();

    const unsigned int msgCount = 50;

    for( unsigned int i = 0; i < msgCount; ++i )
    {
        std::ostringstream stream;

        stream << "This is test message #" << i << std::ends;

        injectTextMessage( stream.str() , *topic1, consumer1->getConsumerId() );
    }

    for( unsigned int i = 0; i < msgCount; ++i )
    {
        std::ostringstream stream;

        stream << "This is test message #" << i << std::ends;

        injectTextMessage( stream.str() , *topic2, consumer2->getConsumerId() );
    }

    synchronized( &msgListener1.mutex )
    {
        const unsigned int interval = msgCount + 10;
        unsigned int count = 0;

        while( msgListener1.messages.size() != msgCount &&
               count < interval )
        {
            msgListener1.mutex.wait( 3000 );

            ++count;
        }
    }

    CPPUNIT_ASSERT( msgListener1.messages.size() == msgCount );

    synchronized( &msgListener2.mutex )
    {
        const int interval = msgCount + 10;
        int count = 0;

        while( msgListener2.messages.size() != msgCount &&
               count < interval )
        {
            msgListener2.mutex.wait( 3000 );

            ++count;
        }
    }

    CPPUNIT_ASSERT( msgListener2.messages.size() == msgCount );

    msgListener1.clear();
    msgListener2.clear();

    session->rollback();

    synchronized( &msgListener1.mutex )
    {
        const int interval = msgCount + 10;
        int count = 0;

        while( msgListener1.messages.size() != msgCount &&
               count < interval )
        {
            msgListener1.mutex.wait( 3000 );

            ++count;
        }
    }

    CPPUNIT_ASSERT( msgListener1.messages.size() == msgCount );

    synchronized( &msgListener2.mutex )
    {
        const int interval = msgCount + 10;
        int count = 0;

        while( msgListener2.messages.size() != msgCount &&
               count < interval )
        {
            msgListener2.mutex.wait( 3000 );

            ++count;
        }
    }

    CPPUNIT_ASSERT( msgListener2.messages.size() == msgCount );

    delete topic1;
    delete topic2;

    delete consumer1;
    delete consumer2;

    delete session;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionTest::testExpiration()
{
    MyCMSMessageListener msgListener1;
    MyCMSMessageListener msgListener2;

    CPPUNIT_ASSERT( connection != NULL );

    // Create an Auto Ack Session
    cms::Session* session = connection->createSession();

    // Create a Topic
    cms::Topic* topic1 = session->createTopic( "TestTopic1");
    cms::Topic* topic2 = session->createTopic( "TestTopic2");

    CPPUNIT_ASSERT( topic1 != NULL );
    CPPUNIT_ASSERT( topic2 != NULL );

    // Create a consumer
    ActiveMQConsumer* consumer1 =
        dynamic_cast<ActiveMQConsumer*>( session->createConsumer( topic1 ) );
    ActiveMQConsumer* consumer2 =
        dynamic_cast<ActiveMQConsumer*>( session->createConsumer( topic2 ) );

    CPPUNIT_ASSERT( consumer1 != NULL );
    CPPUNIT_ASSERT( consumer2 != NULL );

    consumer1->setMessageListener( &msgListener1 );
    consumer2->setMessageListener( &msgListener2 );

    injectTextMessage( "This is a Test 1" ,
                       *topic1,
                       consumer1->getConsumerId(),
                       decaf::util::Date::getCurrentTimeMilliseconds(),
                       50 );

    synchronized( &msgListener1.mutex )
    {
        if( msgListener1.messages.size() == 0 )
        {
            msgListener1.mutex.wait( 3000 );
        }
    }

    CPPUNIT_ASSERT( msgListener1.messages.size() == 1 );

    injectTextMessage( "This is a Test 2" ,
                       *topic2,
                       consumer2->getConsumerId(),
                       decaf::util::Date::getCurrentTimeMilliseconds() - 100,
                       1 );

    synchronized( &msgListener2.mutex )
    {
        if( msgListener2.messages.size() == 0 )
        {
            msgListener2.mutex.wait( 100 );
        }
    }

    CPPUNIT_ASSERT( msgListener2.messages.size() == 0 );

    cms::TextMessage* msg1 =
        dynamic_cast< cms::TextMessage* >(
            msgListener1.messages[0] );

    CPPUNIT_ASSERT( msg1 != NULL );

    std::string text1 = msg1->getText();

    CPPUNIT_ASSERT( text1 == "This is a Test 1" );

    delete topic1;
    delete topic2;

    delete consumer1;
    delete consumer2;

    delete session;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionTest::setUp()
{
    try
    {
        ActiveMQConnectionFactory factory("mock://127.0.0.1:12345?wireFormat=openwire");

        connection = dynamic_cast< ActiveMQConnection*>(
            factory.createConnection() );

        // Get the Transport and make sure we got a dummy Transport
        dTransport = transport::mock::MockTransport::getInstance();
        CPPUNIT_ASSERT( dTransport != NULL );

        connection->setExceptionListener( &exListener );
        connection->start();
    }
    catch(...)
    {
        bool exceptionThrown = false;

        CPPUNIT_ASSERT( exceptionThrown );
    }
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionTest::tearDown() {
    delete connection;
}

////////////////////////////////////////////////////////////////////////////////
void ActiveMQSessionTest::injectTextMessage( const std::string message,
                                             const cms::Destination& destination,
                                             const commands::ConsumerId& id,
                                             const long long timeStamp,
                                             const long long timeToLive )
{
    Pointer<ActiveMQTextMessage> msg( new ActiveMQTextMessage() );

    Pointer<ProducerId> producerId( new ProducerId() );
    producerId->setConnectionId( id.getConnectionId() );
    producerId->setSessionId( id.getSessionId() );
    producerId->setValue( 1 );

    Pointer<MessageId> messageId( new MessageId() );
    messageId->setProducerId( producerId );
    messageId->setProducerSequenceId( 2 );

    // Init Message
    msg->setText( message.c_str() );
    msg->setCMSDestination( &destination );
    msg->setCMSMessageID( "Id: 123456" );
    msg->setMessageId( messageId );

    long long expiration = 0LL;

    if( timeStamp != 0 ) {
        msg->setCMSTimestamp( timeStamp );

        if( timeToLive > 0LL ) {
            expiration = timeToLive + timeStamp;
        }
    }

    msg->setCMSExpiration( expiration );

    // Send the Message
    CPPUNIT_ASSERT( dTransport != NULL );

    Pointer<MessageDispatch> dispatch( new MessageDispatch() );
    dispatch->setMessage( msg );
    dispatch->setConsumerId( Pointer<ConsumerId>( id.cloneDataStructure() ) );

    dTransport->fireCommand( dispatch );
}
