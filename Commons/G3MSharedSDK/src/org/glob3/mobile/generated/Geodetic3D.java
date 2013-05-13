

package org.glob3.mobile.generated;

//
//  Geodetic3D.cpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 31/05/12.
//  Copyright (c) 2012 IGO Software SL. All rights reserved.
//

//
//  Geodetic3D.hpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 31/05/12.
//  Copyright (c) 2012 IGO Software SL. All rights reserved.
//


/**
 * Class to represent a position in the globe by latitude, longitud and altitude.
 */
public class Geodetic3D {
   private final Angle  _latitude;
   private final Angle  _longitude;
   private final double _height;


   public void dispose() {
   }


   public static Geodetic3D nan() {
      return new Geodetic3D(Angle.nan(), Angle.nan(), 0);
   }


   public final boolean isNan() {
      return _latitude.isNan() || _longitude.isNan();
   }


   public static Geodetic3D zero() {
      return new Geodetic3D(Angle.zero(), Angle.zero(), 0);
   }


   public static Geodetic3D fromDegrees(final double lat,
                                        final double lon,
                                        final double height) {
      return new Geodetic3D(Angle.fromDegrees(lat), Angle.fromDegrees(lon), height);
   }


   public static Geodetic3D linearInterpolation(final Geodetic3D from,
                                                final Geodetic3D to,
                                                final double alpha) {
      return new Geodetic3D(Angle.linearInterpolation(from.latitude(), to.latitude(), alpha), Angle.linearInterpolation(
               from.longitude(), to.longitude(), alpha), IMathUtils.instance().linearInterpolation(from.height(), to.height(),
               alpha));
      //((1.0 - alpha) * from.height()) + (alpha * to.height())
   }


   public Geodetic3D(final Angle latitude,
                     final Angle longitude,
                     final double height) {
      _latitude = new Angle(latitude);
      _longitude = new Angle(longitude);
      _height = height;
   }


   public Geodetic3D(final Geodetic2D g2,
                     final double height) {
      _latitude = new Angle(g2.latitude());
      _longitude = new Angle(g2.longitude());
      _height = height;
   }


   public Geodetic3D(final Geodetic3D g) {
      _latitude = new Angle(g._latitude);
      _longitude = new Angle(g._longitude);
      _height = g._height;
   }


   public final Angle latitude() {
      return _latitude;
   }


   public final Angle longitude() {
      return _longitude;
   }


   public final double height() {
      return _height;
   }


   public final Geodetic2D asGeodetic2D() {
      return new Geodetic2D(_latitude, _longitude);
   }


   public final String description() {
      final IStringBuilder isb = IStringBuilder.newStringBuilder();
      isb.addString("(lat=");
      isb.addString(_latitude.description());
      isb.addString(", lon=");
      isb.addString(_longitude.description());
      isb.addString(", height=");
      isb.addDouble(_height);
      isb.addString(")");
      final String s = isb.getString();
      if (isb != null) {
         isb.dispose();
      }
      return s;
   }


   public final Geodetic3D add(final Geodetic3D that) {
      return new Geodetic3D(_latitude.add(that._latitude), _longitude.add(that._longitude), _height + that._height);
   }


   public final Geodetic3D sub(final Geodetic3D that) {
      return new Geodetic3D(_latitude.sub(that._latitude), _longitude.sub(that._longitude), _height - that._height);
   }


   public final Geodetic3D times(final double magnitude) {
      return new Geodetic3D(_latitude.times(magnitude), _longitude.times(magnitude), _height * magnitude);
   }


   public final Geodetic3D div(final double magnitude) {
      return new Geodetic3D(_latitude.div(magnitude), _longitude.div(magnitude), _height / magnitude);
   }


   /**
    * Given a start point, initial bearing, and distance, this will calculate the destination point and final bearing travelling
    * along a (shortest distance) great circle arc. See http: //www.movable-type.co.uk/scripts/latlong.html
    * 
    */

   public static Geodetic3D calculateRhumbLineDestination(final Geodetic3D position,
                                                          final double distance,
                                                          final double R,
                                                          Angle bearing) {

      bearing = bearing.normalized();

      final double angularDistance = distance / (R + position.height());

      final double dLat = angularDistance * bearing.cosinus();

      final double nLatRadians = position.latitude().radians() + dLat;
      final Angle aLat = Angle.fromRadians(nLatRadians);

      final double tg1 = IMathUtils.instance().tan((IMathUtils.instance().pi() / 4) + (aLat.radians() / 2));
      final double tg2 = IMathUtils.instance().tan((IMathUtils.instance().pi() / 4) + position.latitude().radians());

      final double stretchedLatitudeDifference = IMathUtils.instance().log(tg1 / tg2);
      ILogger.instance().logWarning("stretchedLatitudeDifference: %", stretchedLatitudeDifference);

      double q;
      if ((dLat != 0) && !IMathUtils.instance().isNan(stretchedLatitudeDifference)) {
         q = dLat / stretchedLatitudeDifference;
      }
      else {
         q = position.latitude().cosinus();
      }

      final double dLon = (angularDistance * bearing.sinus()) / q;
      //TODO: this
      //// check for some daft bugger going past the pole, normalise latitude if so
      //if (Math.abs(lat2) > Math.PI/2) lat2 = lat2>0 ? Math.PI-lat2 : -Math.PI-lat2;

      final double a = position.longitude().radians() + dLon + IMathUtils.instance().pi();
      final double nLonRadians = IMathUtils.instance().fmod(a, (2 * IMathUtils.instance().pi())) - IMathUtils.instance().pi();

      final Angle aLon = Angle.fromRadians(nLonRadians);


      final Geodetic3D destination = new Geodetic3D(aLat, aLon, position.height());
      //Only for debug
      ILogger.instance().logWarning("Initial Position: " + position.description());
      ILogger.instance().logWarning("Destination Position: " + destination.description());

      final Angle angle = Geodetic2D.bearing(destination.asGeodetic2D(), position.asGeodetic2D());

      ILogger.instance().logWarning(
               "Angle between obj and behin position: " + angle.description() + ". Diference with heading: "
                        + angle.sub(bearing).description());

      return destination;
   }


   public static Geodetic3D calculateInverseRhumbLineDestination(final Geodetic3D position,
                                                                 final double distance,
                                                                 final double R,
                                                                 final Angle bearing) {
      return calculateRhumbLineDestination(position, distance, R, bearing.add(Angle.fromDegrees(180)));
   }


}
