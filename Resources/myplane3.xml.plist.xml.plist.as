package
{
	import Box2D.Dynamics.*;
	import Box2D.Collision.*;
	import Box2D.Collision.Shapes.*;
	import Box2D.Common.Math.*;
    import flash.utils.Dictionary;

    public class PhysicsData extends Object
	{
		// ptm ratio
        public var ptm_ratio:Number = 32;
		
		// the physcis data 
		var dict:Dictionary;
		
        //
        // bodytype:
        //  b2_staticBody
        //  b2_kinematicBody
        //  b2_dynamicBody

        public function createBody(name:String, world:b2World, bodyType:uint, userData:*):b2Body
        {
            var fixtures:Array = dict[name];

            var body:b2Body;
            var f:Number;

            // prepare body def
            var bodyDef:b2BodyDef = new b2BodyDef();
            bodyDef.type = bodyType;
            bodyDef.userData = userData;

            // create the body
            body = world.CreateBody(bodyDef);

            // prepare fixtures
            for(f=0; f<fixtures.length; f++)
            {
                var fixture:Array = fixtures[f];

                var fixtureDef:b2FixtureDef = new b2FixtureDef();

                fixtureDef.density=fixture[0];
                fixtureDef.friction=fixture[1];
                fixtureDef.restitution=fixture[2];

                fixtureDef.filter.categoryBits = fixture[3];
                fixtureDef.filter.maskBits = fixture[4];
                fixtureDef.filter.groupIndex = fixture[5];
                fixtureDef.isSensor = fixture[6];

                if(fixture[7] == "POLYGON")
                {                    
                    var p:Number;
                    var polygons:Array = fixture[8];
                    for(p=0; p<polygons.length; p++)
                    {
                        var polygonShape:b2PolygonShape = new b2PolygonShape();
                        polygonShape.SetAsArray(polygons[p], polygons[p].length);
                        fixtureDef.shape=polygonShape;

                        body.CreateFixture(fixtureDef);
                    }
                }
                else if(fixture[7] == "CIRCLE")
                {
                    var circleShape:b2CircleShape = new b2CircleShape(fixture[9]);                    
                    circleShape.SetLocalPosition(fixture[8]);
                    fixtureDef.shape=circleShape;
                    body.CreateFixture(fixtureDef);                    
                }                
            }

            return body;
        }

		
        public function PhysicsData(): void
		{
			dict = new Dictionary();
			

			dict["bat_penguind_0"] = [

										[
											// density, friction, restitution
                                            1, 0.7, 0.2,
                                            // categoryBits, maskBits, groupIndex, isSensor
											1, 65535, 0, false,
											'POLYGON',

                                            // vertexes of decomposed polygons
                                            [

                                                [   new b2Vec2(76/ptm_ratio, 30/ptm_ratio)  ,  new b2Vec2(60/ptm_ratio, 10.5/ptm_ratio)  ,  new b2Vec2(80/ptm_ratio, 9.5/ptm_ratio)  ,  new b2Vec2(91.5/ptm_ratio, 34/ptm_ratio)  ] ,
                                                [   new b2Vec2(10.5/ptm_ratio, 10/ptm_ratio)  ,  new b2Vec2(30/ptm_ratio, 9.5/ptm_ratio)  ,  new b2Vec2(14.5/ptm_ratio, 29/ptm_ratio)  ,  new b2Vec2(0/ptm_ratio, 35/ptm_ratio)  ] ,
                                                [   new b2Vec2(30/ptm_ratio, 9.5/ptm_ratio)  ,  new b2Vec2(43/ptm_ratio, -0.5/ptm_ratio)  ,  new b2Vec2(52/ptm_ratio, 1/ptm_ratio)  ,  new b2Vec2(76/ptm_ratio, 30/ptm_ratio)  ,  new b2Vec2(66/ptm_ratio, 34/ptm_ratio)  ,  new b2Vec2(46/ptm_ratio, 38.5/ptm_ratio)  ,  new b2Vec2(14.5/ptm_ratio, 29/ptm_ratio)  ]
											]

										]

									];

		}
	}
}
