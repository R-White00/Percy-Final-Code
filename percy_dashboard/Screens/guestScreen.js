import { StatusBar } from 'expo-status-bar';
import { StyleSheet, Text, View, SafeAreaView, TextInput, TouchableOpacity,Pressable} from 'react-native';
import React, {useState, useEffect} from 'react';

import Ionicons from '@expo/vector-icons/Ionicons';
import { FontAwesome } from '@expo/vector-icons';
import { MaterialCommunityIcons } from '@expo/vector-icons';
import { MaterialIcons } from '@expo/vector-icons';
import BasicSpeedometer from 'react-native-speedometer-chart';
import { Line, Text as SvgText, G } from 'react-native-svg'
import { Button } from 'react-native-paper';
// import RNSpeedometer from 'react-native-speedometer'
import { initializeApp } from "firebase/app";
import { getAnalytics } from "firebase/analytics";
import {collection, getFirestore, onSnapshot, doc, setDoc}  from "firebase/firestore";
import Speedometer , {
  Background,
  Arc,
  Needle,
  Progress,
  Marks,
  Indicator,
  DangerPath,
} from 'react-native-cool-speedometer';


const firebaseConfig = {
  apiKey: "AIzaSyCqnxinzpFRFkfIIShDvlw8uVr_ULMm2CA",
  authDomain: "percy-dashboard.firebaseapp.com",
  projectId: "percy-dashboard",
  storageBucket: "percy-dashboard.appspot.com",
  messagingSenderId: "396244618046",
  appId: "1:396244618046:web:61eecd0942620697c8fa6f",
  measurementId: "G-ETGNWVXGB0"
};

const app = initializeApp(firebaseConfig);
const analytics = getAnalytics(app);
const db = getFirestore(app);
const commandRef = doc(db, "Commands", "PercyCommands");
const deltaRef = doc(db, "Commands", "DeltaCommands");
export default function guestScreen() {

const [batteryValue, setBattery] = useState(100);
const [humidityValue, setHumidity]= useState(0);
const [pressureValue, setPressure]= useState(0);
const [tempValue, setTemp]= useState(0);
const [lightValue, setLightValue]= useState(0);
const [moistureValue, setMoisture]= useState(0);
const [volatilesValue, setVolatiles]= useState(0);
const [percyOp, setPercyOp] = useState(true);
const [deltaOp, setDeltaOp] = useState(false);
const [navOp, setNavOp] = useState(false);
const[paused, setPaused] = useState(false);
const[moveMod, setMoveMod] = useState(10);
const [stowedVal, setStowed] = useState(true);
const [activateVal, setActivate] = useState(false);
const [xVal, setX] = useState(0);
const [yVal, setY] = useState(0);
const [zVal, setZ] = useState(0);

  useEffect(() => {
    const unsubscribe = onSnapshot(collection(db, "Sensors"), (querySnapshot)=>{
      const list=[];
      querySnapshot.forEach((doc) => {
        const{Voltage,Humidity,IR,UV,lightIntensity, Moisture,Pressure,Temperature, TimeStamp,VOC} = doc.data();
        list.push({Voltage,Humidity,IR,UV,lightIntensity,Moisture,Pressure,Temperature,TimeStamp,VOC});
   

      })
        //  console.log("list");
        // console.log(list);
        // console.log("Latest Time");
        // console.log(list[list.length-1].TimeStamp.seconds);
        DisplayVals(list);


    })
    return  unsubscribe


  }, [])





  //Function that looks at timestamps in objects finds the latest keeps, sets that objects values to the displayed values
  const DisplayVals =  (data) => {
    var largestTimeIndex = 0;
    data.forEach((element, index) => {
      if(element.TimeStamp.seconds > data[largestTimeIndex].TimeStamp.seconds ){
        largestTimeIndex = index;
      }

    })
    console.log("Latest Humidity");
    console.log(data[largestTimeIndex].Humidity);
    // console.log("Largest Index ");
    // console.log(largestTimeIndex);
    setHumidity(data[largestTimeIndex].Humidity);
    setLightValue(data[largestTimeIndex].UV)
    setMoisture(data[largestTimeIndex].Moisture)
    setPressure(data[largestTimeIndex].Pressure)
    setVolatiles(data[largestTimeIndex].VOC)
    setTemp(data[largestTimeIndex].Temperature)
    setBattery(data[largestTimeIndex].Voltage)

  }

  const sendCommands = (cmd)=>{
    if(percyOp == true){
      setPaused(false);
      setDoc(commandRef, {Pause:false}, {merge:true});
    if(cmd == "forward"){
      console.log("set forward to true");
      setDoc(commandRef, {Forward:true}, {merge:true});
    } else if (cmd == "null") {
      console.log("set nothing to true");
      setDoc(commandRef, {Forward:false}, {merge:true});
      setDoc(commandRef, {Reverse:false}, {merge:true});
      setDoc(commandRef, {Left:false}, {merge:true});
      setDoc(commandRef, {Right:false}, {merge:true});
      setDoc(commandRef, {Pause:false}, {merge:true});

      
    } else if (cmd == "reverse") {
      console.log("set nothing to true");
      setDoc(commandRef, {Reverse:true}, {merge:true});
    } else if (cmd == "left") {
      console.log("set nothing to true");
      setDoc(commandRef, {Left:true}, {merge:true});
    } else if (cmd == "right") {
      console.log("set nothing to true");
      setDoc(commandRef, {Right:true}, {merge:true});
    } 
  } else if(navOp==true)
  {
    if (cmd == "pause") {
      console.log(!paused);
      

      setDoc(commandRef, {Pause:!paused}, {merge:true});
    }
  } else if (deltaOp == true){
    if(!stowedVal){
    if(cmd == "forward"){
      console.log("set forward to true");
      setY(yVal + moveMod);
      setDoc(deltaRef, {Y:yVal}, {merge:true});
    } else if (cmd == "null") {
      console.log("set nothing to true");
     
      
    } else if (cmd == "reverse") {
      console.log("set nothing to true");
      setY(yVal - moveMod);
      setDoc(deltaRef, {Y:yVal}, {merge:true});
    } else if (cmd == "left") {
      setX(xVal - moveMod);
      setDoc(deltaRef, {X:xVal}, {merge:true});
      console.log("set nothing to true");
      
    } else if (cmd == "right") {
      console.log("set nothing to true");
      setX(xVal + moveMod);
      console.log(xVal);
      setDoc(deltaRef, {X:xVal}, {merge:true});
    
    }else if (cmd == "up") {
      console.log("Z pressed");
      setZ(zVal + moveMod);
      setDoc(deltaRef, {Z:zVal}, {merge:true});
    }else if (cmd == "down") {
      console.log("set nothing to true");
       setZ(zVal - moveMod);
      setDoc(deltaRef, {Z:zVal}, {merge:true});
    }
    else if(cmd == "activate"){
      setActivate(!activateVal);
      
    }
    else if(cmd == "deactivate"){
    }else if (cmd == "pause") {
      console.log("Home")
      setZ(0);
      setX(0);
      setY(0);
      

      // setDoc(commandRef, {Y:0}, {merge:true});
    }
  }
    if (cmd=="stow"){
      console.log(stowedVal);
        setStowed(!stowedVal);
        
        
    }
  }

  }

  useEffect(()=>{

    setDoc(deltaRef, {Z:zVal}, {merge:true}); 
    setDoc(deltaRef, {Y:yVal}, {merge:true}); 
    setDoc(deltaRef, {X:xVal}, {merge:true}); 
    setDoc(deltaRef, {Activate:activateVal}, {merge:true});
    setDoc(deltaRef, {Stowed:stowedVal}, {merge:true});
  })

 function RenderPauseButton(){
   if(navOp){
if(paused != true){
  // console.log(paused);
  return <Ionicons name="pause-circle" size={200} color="#19c416" />;
}else{
  return <Ionicons name="play-circle-sharp" size={200} color="#19c416" />;
}

} else if(deltaOp){
  return <MaterialCommunityIcons name="home-circle" size={200} color="#19c416" />
}else{
  return <Ionicons name="pause-circle" size={200} color="#19c416" />;
}
 }

 function RenderMoveModifiers(){
   if(deltaOp){
     return (
       
      <View style={{paddingVertical:40, justifyContent:"center", flexDirection:'row'}}>
      <View style={{paddingHorizontal:20,}}>
    <Button mode="contained" onPress={() => {console.log('Pressed'); setMoveMod(10)}}  color={(moveMod==10)
        ? 'rgba(3, 252, 61,0.5)'
        : 'rgba(3, 252, 61,1)'}
     contentStyle={{height:70,width:"100%",justifyContent: 'center',
     alignItems: 'center',}}>
      10mm
      </Button>
      </View>
      <Button mode="contained" onPress={() => {console.log('Pressed'); setMoveMod(100)}}  color={(moveMod==100)
        ? 'rgba(3, 252, 61,0.5)'
        : 'rgba(3, 252, 61,1)'}
     contentStyle={{height:70,width:"100%",justifyContent: 'center',
     alignItems: 'center',}}>
      100mm
      </Button> 

    </View>
     );

   }else{
     return <View></View>
   }
 }

  const center = 250 / 2
  return (
    <SafeAreaView style={{flex:1}}>
    <View style={styles.container}>

<View style={styles.topBar}>
  <View style={{flexDirection:"row", alignItems:"center",flexWrap:"wrap"}}>
    <View style={{flex:1, }}>

{/* Border */}
  <Text style={{color:"#fff", fontSize:70, fontWeight:'500', }}>
    Percy Dashboard
  </Text>
    </View>

          <View style={{flex:1, alignItems:"flex-end", flexWrap:"wrap"}}>
            <View style={{flex:1, flexDirection:"row",paddingEnd:40}}> 
            <View style={{alignItems:"center", paddingEnd:50}}>

            <Text style={{color:"#fff", fontSize:30, fontWeight:'400', textAlign:'center'}}>
              Number of {'\n'}
              Crops Targeted:
            </Text>
            <View style={{flexDirection:"row"}}> 
            <MaterialCommunityIcons name="tree" size={50} color="white" style={{alignContent:'flex-end'}}/>
            
            <Text style={{color:"#fff", fontSize:30,}}>    1</Text>
            </View>
            </View>
            <View style={{alignItems:"center"}}>

            <Text style={{color:"#fff", fontSize:30, fontWeight:'400', textAlign:'center'}}>
              Battery {'\n'}
              {batteryValue}%:
            </Text>
            <View style={{flexDirection:"row", }}> 
            
            <MaterialCommunityIcons name="battery-outline" size={50} color="white" />    
                    </View>
            </View>
            </View>
          </View>
     </View>
  
  </View>


      <View style={{ flex: 5, backgroundColor: "white", flexDirection:"row" }} >
        <View style={{ flex:4.5, backgroundColor:"white", flexWrap:'wrap' }}>
          <View style={{flexWrap:'wrap' ,flexDirection:'row', flex:1, backgroundColor:"white", justifyContent:"space-evenly", alignItems:"center", alignContent:"center", }}>
          <View style={{marginTop:90, alignItems:"center"}}>
          <Text style={{paddingBottom:30}}>Pressure</Text>
          {/* Pressure Dial */}
          <Speedometer
                value={pressureValue}
                max={1300}
                angle={160}
                fontFamily='squada-one'
                accentColor="#19c416"
              >

                <Background angle={180} color="#fff" />
                <Arc/>
              <Needle color="#19c416"/>
                <Progress/>
                {/* <Marks lineColor="black" fill="black" /> */}
                <Marks step={100} lineSize={20}>
                      {(mark, i) => (
                        <G key={i}>
                          {mark.isEven && (
                            <SvgText
                              {...mark.textProps}
                              textAnchor="middle"
                              alignmentBaseline="middle"
                              fontSize={18}
                              opacity={0.6}
                              fill="#19c416"
                              
                            >
                              {mark.value}
                            </SvgText>
                          )}
                          <Line
                            {...mark.coordinates}
                            stroke="#19c416"
                            strokeOpacity={1}
                            strokeWidth={5}
                          />
                        </G>
                      )}
                    </Marks>
                    <Indicator>
    {(value, textProps) => (
      <SvgText
        {...textProps}
        fontSize={30}
        fill="#black"
        x={250 / 2}
        y={170}
        textAnchor="middle"
        fontFamily='squada-one'
        color="black"
      >
        {value} hPa
      </SvgText>
    )}
  </Indicator>
          </Speedometer> 

          </View>


          
          <View style={{alignItems:"center"}}>
            {/* Humidity Dial */}
          <Text style={{paddingBottom:30}}>Humidity</Text>
          <BasicSpeedometer value={humidityValue} totalValue={100} internalColor="#19c416" showText text= {humidityValue + "%"} textStyle={{ color: 'black', fontSize:20 }}/> 
          </View>         

          </View>
          <View style={{flexWrap:'wrap' ,flexDirection:'row', flex:1, backgroundColor:"white", justifyContent:"space-evenly", alignItems:"center", alignContent:'space-around',}}>
         
          <View style={{marginTop:90, alignItems:"center"}}>
          <Text style={{paddingBottom:30}}>Soil Moisture</Text>
          {/* Soil Moisture Dial */}
          <Speedometer
                value={moistureValue}
                max={100}
                angle={160}
                fontFamily='squada-one'
                accentColor="#19c416"
              >

                <Background angle={180} color="#fff" />
                <Arc/>
              <Needle color="#19c416"/>
                <Progress/>
                {/* <Marks lineColor="black" fill="black" /> */}
                <Marks step={10} lineSize={20}>
                      {(mark, i) => (
                        <G key={i}>
                          {mark.isEven && (
                            <SvgText
                              {...mark.textProps}
                              textAnchor="middle"
                              alignmentBaseline="middle"
                              fontSize={18}
                              opacity={0.6}
                              fill="#19c416"
                              
                            >
                              {mark.value}
                            </SvgText>
                          )}
                          <Line
                            {...mark.coordinates}
                            stroke="#19c416"
                            strokeOpacity={1}
                            strokeWidth={5}
                          />
                        </G>
                      )}
                    </Marks>
                    <Indicator>
    {(value, textProps) => (
      <SvgText
        {...textProps}
        fontSize={30}
        fill="#black"
        x={250 / 2}
        y={170}
        textAnchor="middle"
        fontFamily='squada-one'
        color="black"
      >
        {value}%
      </SvgText>
    )}
  </Indicator>
  <DangerPath/>
          </Speedometer> 

          </View>


          <View style={{alignItems:"center"}}>
          {/* UV light Dial */}
          <Text style={{paddingBottom:30}}>UV Light</Text>
          <BasicSpeedometer value={lightValue} totalValue={10} internalColor="#19c416" showText text={lightValue} textStyle={{ color: 'black', fontSize:20 }}/> 
          </View>              </View>
            <View style={{flexWrap:'wrap' ,flexDirection:'row', flex:1, backgroundColor:"white", justifyContent:"space-evenly", alignItems:"center",}}>
            <View style={{alignItems:"center"}}>
            <Text style={{paddingBottom:30}}>Temperature</Text>

          {/* Temperature Dial */}
            <Speedometer
      value={tempValue}
      min={-50}
      max={50}
      angle={320}
      accentColor="#19c416"

    >
      <Background
        color="#fff"
        opacity={0.3}
      />
      <Progress/>
      <Marks step={5}>
        {(mark, i) => (
          <G key={i}>
            {mark.isEven && (
              <SvgText
                {...mark.textProps}
                textAnchor="middle"
                alignmentBaseline="middle"
                fontSize={18}
                opacity={0.6}
                fill="black"
                
              >
                {mark.value}°
              </SvgText>
            )}
            <Line
              {...mark.coordinates}
              stroke="black"
              strokeOpacity={0.4}
            />
          </G>
        )}
      </Marks>
      <Needle baseOffset={-30} circleRadius={0} color="#19c416" offset={50} />
      <Indicator>
        {(value, textProps) => (
          <SvgText
            {...textProps}
            fontSize={40}
            x={center}
            y={center + 10}
            textAnchor="middle"
            alignmentBaseline="middle"
          >
            {value}°C
          </SvgText>
        )}
      </Indicator>
     
    </Speedometer>

            </View>

            <View style={{alignItems:"center"}}>

{/* Volatiles Dial */}
<Text style={{paddingBottom:30}}>Volatiles </Text>
<BasicSpeedometer value={volatilesValue} totalValue={100} internalColor="#19c416" showText text={volatilesValue + " ppm"} textStyle={{ color: 'black', fontSize:20 }}/> 
</View>              </View>
 
        </View>
    

        

        

        </View>
      
    </View>


    </SafeAreaView>
  );
}

const styles = StyleSheet.create({
  container: {


flex: 1,
  },
  topBar: {
  flex:1,
  flexWrap:"wrap",
  backgroundColor: '#19c416',
  justifyContent: 'center',
    
  },
  bottom:{
flex:3,
backgroundColor: '#fff',
alignItems: 'center',
justifyContent: 'center',
  },
 
  
});
