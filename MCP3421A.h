#ifndef MCP3421_H
#define	MCP3421_H

#define MCP3421    0b11010000/**MCP3421 device address*/
#define Max_12bit 2047 
#define Max_14bit 8191
#define Max_16bit 32767
#define Max_18bit 131071

typedef enum { CONTI_CONVERTION_MODE    = (uint8_t)(1<<4), 
               SINGLE_CONVERTION_MODE  = (uint8_t)(0<<4)
             } MCP3421_MODE_TypeDef;

typedef enum { SPS240_12bit   = (uint8_t)(0b00&(3<<2)), 
               SPS60_14bit    = (uint8_t)(0b01&(3<<2)),
               SPS15_16bit    = (uint8_t)(0b10&(3<<2)),
               SPS4_18bit     = (uint8_t)(0b11&(3<<2))
             } MCP3421_SPS_TypeDef;

typedef enum { GAIN1    = (uint8_t)(0b00&(3<<0)), 
               GAIN2    = (uint8_t)(0b01&(3<<0)),
               GAIN4    = (uint8_t)(0b10&(3<<0)),
               GAIN8    = (uint8_t)(0b11&(3<<0))
             } MCP3421_GAIN_TypeDef; 

           
#endif