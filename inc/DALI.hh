#ifndef __DALI_HH
#define __DALI_HH
#include <iostream>
#include <vector>
#include <cstdlib>
#include <math.h>

#include "TObject.h"
#include "TVector3.h"
#include "TMath.h"
#include "DALIdefs.h"
using namespace std;

/*!
  Container for the DALI information
*/
class DALIHit : public TObject
{
public:
  //! default constructor
  DALIHit()
  {
    Clear();
  }
  //! constructor with individual values
  DALIHit(short id, double en, double DCen, TVector3 pos, double time, double toffset, unsigned short hitsadded)
  {
    fID = id;
    fpos = pos;
    fen = en;
    fDCen = DCen;
    ftime = time;
    ftoffset = toffset;
    fhitsadded = hitsadded;
  }
  //! Clear the music information
  void Clear(Option_t *option = "")
  {
    fID = -1.;
    fpos.SetXYZ(0, 0, 0);
    fen = sqrt(-1.);
    fDCen = sqrt(-1.);
    ftime = sqrt(-1.);
    ftoffset = sqrt(-1.);
    fhitsadded = 0;
    fadc = -1;
    ftdc = -1;
  }
  //! Set the detector ID
  void SetID(short id)
  {
    if (id < MAXNCRYSTAL) // id is from 0
      fID = id;
    fhitsadded++;
  }
  //! Set the position by vector
  void SetPos(TVector3 pos) { fpos = pos; }
  //! Set the position by coordinates
  void SetPos(double x, double y, double z)
  {
    fpos.SetXYZ(x, y, z);
  }
  //! Set the position by vector
  void SetPos(vector<double> r)
  {
    if (r.size() == 3)
      fpos.SetXYZ(r.at(0), r.at(1), r.at(2));
    else
      cout << "error in DALI.SetPos()" << endl;
  }
  //! Set the energy
  void SetEnergy(double energy) { fen = energy; }
  //! Set the Doppler corrected energy
  void SetDCEnergy(double dcen) { fDCen = dcen; }
  //! Set the time
  void SetTime(double time) { ftime = time; }
  //! Set the time after offset correction
  void SetTOffset(double toffset) { ftoffset = toffset; }
  //! Set the raw ADC value
  void SetADC(int adc) { fadc = adc; }
  //! Set the raw TDC value
  void SetTDC(int tdc) { ftdc = tdc; }

  //! Addback a hit
  void AddBackHit(DALIHit *hit)
  {
    if (fen < hit->GetEnergy())
    {
      cout << " error hits not sorted by energy" << endl;
      return;
    }
    fen += hit->GetEnergy();
    fhitsadded += hit->GetHitsAdded();
  }

  //! Get the ID
  short GetID() { return fID; }
  //! Get the position vector
  TVector3 GetPos() { return fpos; }
  //! Get the energy
  double GetEnergy() { return fen; }
  //! Get the Doppler corrected energy
  double GetDCEnergy() { return fDCen; }
  //! Get the time
  double GetTime() { return ftime; }
  //! Get the time after offset correction
  double GetTOffset() { return ftoffset; }
  //! Get the raw ADC value
  int GetADC() { return fadc; }
  //! Get the raw TDC value
  int GetTDC() { return ftdc; }
  //! Get the number of hits that were added back to create one gamma
  unsigned short GetHitsAdded() { return fhitsadded; }

  //! Get Doppler corrected energy with input beta
  double GetDCEnergy(double beta)
  {
    return fen / sqrt(1 - beta * beta) * (1 - beta * cos(fpos.Theta()));
  }
  //! Apply the Doppler correction with the given beta, assuming motion in the +z direction.
  void DopplerCorrect(double beta)
  {
    fDCen = fen / sqrt(1 - beta * beta) * (1 - beta * cos(fpos.Theta()));
    // cout << fen << "\t" << beta << "\t" << fpos.Theta() << "\t" << fDCen << endl;
  }
  //! Apply the Doppler correction with the given beta, and a position, assuming motion in the +z direction.
  void DopplerCorrect(double beta, double zreac)
  {
    fpos.SetZ(fpos.Z() - zreac);
    fDCen = fen / sqrt(1 - beta * beta) * (1 - beta * cos(fpos.Theta()));
  }
  //! Printing information
  void Print(Option_t *option = "") const
  {
    cout << "ID " << fID;
    cout << "\tenergy " << fen;
    cout << "\tDCenergy " << fDCen;
    cout << "\ttheta " << fpos.Theta() * 180. / TMath::Pi();
    cout << "\tphi " << fpos.Phi() * 180. / TMath::Pi();
    cout << "\ttime " << ftime;
    cout << "\ttoffset " << ftoffset;
    cout << "\thits added " << fhitsadded << endl;
    return;
  }

protected:
  //! detector number
  short fID;
  //! the position
  TVector3 fpos;
  //! the energy lab system
  double fen;
  //! the Doppler corrected energy
  double fDCen;
  //! the time
  double ftime;
  //! the time after offset correction
  double ftoffset;
  //! the raw adc value
  int fadc;
  //! the raw tdc value
  int ftdc;
  //! how many hits were added back to create one gamma
  unsigned short fhitsadded;

  /// \cond CLASSIMP
  ClassDef(DALIHit, 1);
  /// \endcond
};
/*!
  Container for the DALI information
*/
class DALI : public TObject
{
public:
  //! default constructor
  DALI()
  {
    Clear();
  }
  //! Clear the music information
  void Clear(Option_t *option = "")
  {
    fmult = 0;
    fmultAB = 0;
    for (vector<DALIHit *>::iterator hit = fhits.begin(); hit != fhits.end(); hit++)
    {
      delete *hit;
    }
    fhits.clear();
    for (vector<DALIHit *>::iterator hitab = fhitsAB.begin(); hitab != fhitsAB.end(); hitab++)
    {
      delete *hitab;
    }
    fhitsAB.clear();
  }
  //! Add a hit
  void AddHit(DALIHit *hit)
  {
    fhits.push_back(hit);
    fmult++;
  }
  //! Add a hit after addback
  void AddHitAB(DALIHit *hit)
  {
    fhitsAB.push_back(hit);
    fmultAB++;
  }
  //! Set all hits
  void SetHits(vector<DALIHit *> hits)
  {
    fmult = hits.size();
    fhits = hits;
  }
  //! Set all hits after addback
  void SetABHits(vector<DALIHit *> hits)
  {
    fmultAB = hits.size();
    fhitsAB = hits;
  }
  //! Do the Doppler correction
  void DopplerCorrect(double beta)
  {
    for (vector<DALIHit *>::iterator hit = fhits.begin(); hit != fhits.end(); hit++)
    {
      (*hit)->DopplerCorrect(beta);
    }
    for (vector<DALIHit *>::iterator hit = fhitsAB.begin(); hit != fhitsAB.end(); hit++)
    {
      (*hit)->DopplerCorrect(beta);
    }
  }
  //! Do the Doppler correction
  void DopplerCorrect(double beta, double zreac)
  {
    for (vector<DALIHit *>::iterator hit = fhits.begin(); hit != fhits.end(); hit++)
    {
      (*hit)->DopplerCorrect(beta, zreac);
    }
    for (vector<DALIHit *>::iterator hit = fhitsAB.begin(); hit != fhitsAB.end(); hit++)
    {
      (*hit)->DopplerCorrect(beta, zreac);
    }
  }
  //! Returns the multiplicity of the event
  unsigned short GetMult() { return fmult; }
  //! Returns the whole vector of hits
  vector<DALIHit *> GetHits() { return fhits; }
  //! Returns the hit number n
  DALIHit *GetHit(unsigned short n) { return fhits.at(n); }
  //! Returns the multiplicity of the event after addback
  int GetMultAB() { return fmultAB; }
  //! Returns the whole vector of hits after addback
  vector<DALIHit *> GetHitsAB() { return fhitsAB; }
  //! Returns the hit number n after addback
  DALIHit *GetHitAB(int n) { return fhitsAB.at(n); }

  //! Printing information
  void Print(Option_t *option = "") const
  {
    cout << "multiplicity " << fmult << " event" << endl;
    for (unsigned short i = 0; i < fhits.size(); i++)
      fhits.at(i)->Print();
    cout << "after addback multiplicity " << fmultAB << endl;
    for (unsigned short i = 0; i < fhitsAB.size(); i++)
      fhitsAB.at(i)->Print();
  }

protected:
  //! multiplicity
  unsigned short fmult;
  //! vector with the hits
  vector<DALIHit *> fhits;
  //! multiplicity after addback
  unsigned short fmultAB;
  //! vector with the hits after addback
  vector<DALIHit *> fhitsAB;

  /// \cond CLASSIMP
  ClassDef(DALI, 1);
  /// \endcond
};

/*!
  Compare two hits by their energies
*/
class HitComparer
{
public:
  //! compares energies of the hits
  bool operator()(DALIHit *lhs, DALIHit *rhs)
  {
    return (*lhs).GetEnergy() > (*rhs).GetEnergy();
  }
};
#endif

// #ifndef __DALI_HH
// #define __DALI_HH

// #include <iostream>
// #include <vector>
// #include <cstdlib>
// #include <cmath>
// #include <limits>

// #include "TObject.h"
// #include "TVector3.h"
// #include "TMath.h"
// #include "DALIdefs.h"

// using std::cout;
// using std::endl;
// using std::vector;

// /*!
//   Container for the DALI information
// */
// class DALIHit : public TObject
// {
// public:
//   //! default constructor
//   DALIHit() { Clear(); }

//   //! constructor with individual values
//   DALIHit(short id, double en, double DCen, TVector3 pos, double time, double toffset, unsigned short hitsadded)
//   {
//     fID = id;
//     fpos = pos;
//     fen = en;
//     fDCen = DCen;
//     ftime = time;
//     ftoffset = toffset;
//     fhitsadded = hitsadded;
//     fadc = -1;
//     ftdc = -1;
//   }

//   //! copy constructor (deep copy of members)
//   DALIHit(const DALIHit &rhs) : fID(rhs.fID),
//                                 fpos(rhs.fpos),
//                                 fen(rhs.fen),
//                                 fDCen(rhs.fDCen),
//                                 ftime(rhs.ftime),
//                                 ftoffset(rhs.ftoffset),
//                                 fadc(rhs.fadc),
//                                 ftdc(rhs.ftdc),
//                                 fhitsadded(rhs.fhitsadded)
//   {
//   }

//   //! copy assignment
//   DALIHit &operator=(const DALIHit &rhs)
//   {
//     if (this != &rhs)
//     {
//       fID = rhs.fID;
//       fpos = rhs.fpos;
//       fen = rhs.fen;
//       fDCen = rhs.fDCen;
//       ftime = rhs.ftime;
//       ftoffset = rhs.ftoffset;
//       fadc = rhs.fadc;
//       ftdc = rhs.ftdc;
//       fhitsadded = rhs.fhitsadded;
//     }
//     return *this;
//   }

//   //! create a heap copy (useful for callers)
//   DALIHit *Clone() const { return new DALIHit(*this); }

//   virtual ~DALIHit() = default;

//   //! Clear the hit information
//   void Clear(Option_t *option = "") override
//   {
//     fID = -1;
//     fpos.SetXYZ(0., 0., 0.);
//     fen = std::numeric_limits<double>::quiet_NaN();
//     fDCen = std::numeric_limits<double>::quiet_NaN();
//     ftime = std::numeric_limits<double>::quiet_NaN();
//     ftoffset = std::numeric_limits<double>::quiet_NaN();
//     fhitsadded = 0;
//     fadc = -1;
//     ftdc = -1;
//   }

//   //! Set the detector ID (only accept valid range)
//   void SetID(short id)
//   {
//     if (id >= 0 && id < MAXNCRYSTAL)
//       fID = id;
//     else
//       cout << "DALIHit::SetID(): invalid id " << id << endl;
//   }

//   //! Set the position by vector
//   void SetPos(const TVector3 &pos) { fpos = pos; }
//   //! Set the position by coordinates
//   void SetPos(double x, double y, double z) { fpos.SetXYZ(x, y, z); }
//   //! Set the position by std::vector (size must be 3)
//   void SetPos(const vector<double> &r)
//   {
//     if (r.size() == 3)
//       fpos.SetXYZ(r.at(0), r.at(1), r.at(2));
//     else
//       cout << "DALIHit::SetPos(): error - vector size != 3" << endl;
//   }

//   //! Setters
//   void SetEnergy(double energy) { fen = energy; }
//   void SetDCEnergy(double dcen) { fDCen = dcen; }
//   void SetTime(double time) { ftime = time; }
//   void SetTOffset(double toffset) { ftoffset = toffset; }
//   void SetADC(int adc) { fadc = adc; }
//   void SetTDC(int tdc) { ftdc = tdc; }

//   //! Addback a hit (merge energies and counts). Assumes caller manages position/time merging if needed.
//   void AddBackHit(const DALIHit *hit)
//   {
//     if (!hit)
//       return;
//     fen += hit->GetEnergy();
//     fhitsadded += hit->GetHitsAdded();
//   }

//   //! Getters (const-correct)
//   short GetID() const { return fID; }
//   TVector3 GetPos() const { return fpos; } // returns a copy (TVector3 is small)
//   double GetEnergy() const { return fen; }
//   double GetDCEnergy() const { return fDCen; }
//   double GetTime() const { return ftime; }
//   double GetTOffset() const { return ftoffset; }
//   int GetADC() const { return fadc; }
//   int GetTDC() const { return ftdc; }
//   unsigned short GetHitsAdded() const { return fhitsadded; }

//   //! Compute Doppler-corrected energy using beta (assumes motion along +z)
//   double GetDCEnergy(double beta) const
//   {
//     const double gamma = 1.0 / std::sqrt(1.0 - beta * beta);
//     return fen * gamma * (1.0 - beta * std::cos(fpos.Theta()));
//   }

//   //! Apply Doppler corrections (store result in fDCen). Motion assumed along +z.
//   void DopplerCorrect(double beta)
//   {
//     const double gamma = 1.0 / std::sqrt(1.0 - beta * beta);
//     fDCen = fen * gamma * (1.0 - beta * std::cos(fpos.Theta()));
//   }

//   //! Apply Doppler correction with reaction vertex z-offset
//   void DopplerCorrect(double beta, double zreac)
//   {
//     fpos.SetZ(fpos.Z() - zreac);
//     const double gamma = 1.0 / std::sqrt(1.0 - beta * beta);
//     fDCen = fen * gamma * (1.0 - beta * std::cos(fpos.Theta()));
//   }

//   //! Print info
//   void Print(Option_t *option = "") const override
//   {
//     cout << "ID " << fID;
//     cout << "\tenergy " << fen;
//     cout << "\tDCenergy " << fDCen;
//     cout << "\ttheta " << fpos.Theta() * 180.0 / TMath::Pi();
//     cout << "\tphi " << fpos.Phi() * 180.0 / TMath::Pi();
//     cout << "\ttime " << ftime;
//     cout << "\ttoffset " << ftoffset;
//     cout << "\thits added " << fhitsadded << endl;
//   }

// protected:
//   short fID{-1};                                             //!< detector number
//   TVector3 fpos{};                                           //!< position
//   double fen{std::numeric_limits<double>::quiet_NaN()};      //!< energy (lab)
//   double fDCen{std::numeric_limits<double>::quiet_NaN()};    //!< Doppler-corrected energy
//   double ftime{std::numeric_limits<double>::quiet_NaN()};    //!< time
//   double ftoffset{std::numeric_limits<double>::quiet_NaN()}; //!< time offset corrected
//   int fadc{-1};                                              //!< raw ADC
//   int ftdc{-1};                                              //!< raw TDC
//   unsigned short fhitsadded{0};                              //!< hits added during addback

//   /// \cond CLASSIMP
//   ClassDef(DALIHit, 1);
//   /// \endcond
// };

// /*!
//   Container for the DALI information
// */
// class DALI : public TObject
// {
// public:
//   //! default constructor
//   DALI() { Clear(); }

//   //! no copy (avoid accidental double ownership)
//   DALI(const DALI &) = delete;
//   DALI &operator=(const DALI &) = delete;

//   //! allow move semantics (transfer ownership)
//   DALI(DALI &&other) noexcept
//   {
//     fmult = other.fmult;
//     fhits = std::move(other.fhits);
//     fmultAB = other.fmultAB;
//     fhitsAB = std::move(other.fhitsAB);
//     other.fmult = 0;
//     other.fmultAB = 0;
//   }
//   DALI &operator=(DALI &&other) noexcept
//   {
//     if (this != &other)
//     {
//       Clear();
//       fmult = other.fmult;
//       fhits = std::move(other.fhits);
//       fmultAB = other.fmultAB;
//       fhitsAB = std::move(other.fhitsAB);
//       other.fmult = 0;
//       other.fmultAB = 0;
//     }
//     return *this;
//   }

//   virtual ~DALI() { Clear(); }

//   //! Clear the event (delete owned DALIHit pointers)
//   void Clear(Option_t *option = "") override
//   {
//     // delete and clear primary hits
//     for (DALIHit *h : fhits)
//       delete h;
//     fhits.clear();
//     fmult = 0;

//     // delete and clear addback hits (these are copies owned by fhitsAB)
//     for (DALIHit *h : fhitsAB)
//       delete h;
//     fhitsAB.clear();
//     fmultAB = 0;
//   }

//   //! Add a hit (takes ownership of the pointer)
//   void AddHit(DALIHit *hit)
//   {
//     if (!hit)
//       return;
//     fhits.push_back(hit);
//     ++fmult;
//   }

//   /**
//    * Add a hit after addback.
//    * IMPORTANT: to avoid double ownership/double-delete, we create a copy
//    * of the provided hit and own that copy in fhitsAB. That way callers may
//    * safely call AddHit(dhit); AddHitAB(dhit); without causing double-free.
//    */
//   void AddHitAB(const DALIHit *hit)
//   {
//     if (!hit)
//       return;
//     DALIHit *copy = hit->Clone(); // uses copy constructor
//     fhitsAB.push_back(copy);
//     ++fmultAB;
//   }

//   //! Set all hits (take ownership of the pointers in the provided vector)
//   void SetHits(vector<DALIHit *> hits)
//   {
//     // delete any existing hits first
//     for (DALIHit *h : fhits)
//       delete h;
//     fhits = std::move(hits);
//     fmult = static_cast<unsigned short>(fhits.size());
//   }

//   //! Set all addback hits (take ownership)
//   void SetABHits(vector<DALIHit *> hits)
//   {
//     for (DALIHit *h : fhitsAB)
//       delete h;
//     fhitsAB = std::move(hits);
//     fmultAB = static_cast<unsigned short>(fhitsAB.size());
//   }

//   //! Do the Doppler correction
//   void DopplerCorrect(double beta)
//   {
//     for (DALIHit *h : fhits)
//       if (h)
//         h->DopplerCorrect(beta);
//     for (DALIHit *h : fhitsAB)
//       if (h)
//         h->DopplerCorrect(beta);
//   }

//   //! Do the Doppler correction with z-reaction offset
//   void DopplerCorrect(double beta, double zreac)
//   {
//     for (DALIHit *h : fhits)
//       if (h)
//         h->DopplerCorrect(beta, zreac);
//     for (DALIHit *h : fhitsAB)
//       if (h)
//         h->DopplerCorrect(beta, zreac);
//   }

//   //! Returns the multiplicity of the event
//   unsigned short GetMult() const { return fmult; }

//   //! Returns a const reference to the vector of hits (no copy)
//   const vector<DALIHit *> &GetHits() const { return fhits; }

//   //! Returns the hit number n (throws std::out_of_range if invalid)
//   DALIHit *GetHit(size_t n) const { return fhits.at(n); }

//   //! Returns the multiplicity of the event after addback
//   unsigned short GetMultAB() const { return fmultAB; }

//   //! Returns the whole vector of hits after addback
//   const vector<DALIHit *> &GetHitsAB() const { return fhitsAB; }

//   //! Returns the hit number n after addback
//   DALIHit *GetHitAB(size_t n) const { return fhitsAB.at(n); }

//   //! Printing information
//   void Print(Option_t *option = "") const override
//   {
//     cout << "multiplicity " << fmult << " event" << endl;
//     for (size_t i = 0; i < fhits.size(); ++i)
//       if (fhits[i])
//         fhits[i]->Print();
//     cout << "after addback multiplicity " << fmultAB << endl;
//     for (size_t i = 0; i < fhitsAB.size(); ++i)
//       if (fhitsAB[i])
//         fhitsAB[i]->Print();
//   }

// protected:
//   //! multiplicity
//   unsigned short fmult{0};
//   //! vector with the hits (this object owns the pointers)
//   vector<DALIHit *> fhits;
//   //! multiplicity after addback
//   unsigned short fmultAB{0};
//   //! vector with the hits after addback (owned copies)
//   vector<DALIHit *> fhitsAB;

//   /// \cond CLASSIMP
//   ClassDef(DALI, 1);
//   /// \endcond
// };

// /*!
//   Compare two hits by their energies
// */
// class HitComparer
// {
// public:
//   //! compares energies of the hits (higher energy first)
//   bool operator()(const DALIHit *lhs, const DALIHit *rhs) const
//   {
//     return lhs->GetEnergy() > rhs->GetEnergy();
//   }
// };

// #endif // __DALI_HH
