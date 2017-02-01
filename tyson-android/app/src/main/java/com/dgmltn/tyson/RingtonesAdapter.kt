package com.dgmltn.tyson

import android.content.Context
import android.support.v7.widget.RecyclerView
import android.support.v7.widget.RecyclerView.Adapter
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.RadioButton
import android.widget.TextView

class RingtonesAdapter(context: Context) : Adapter<RingtonesAdapter.ViewHolder>() {

    private val inflater = LayoutInflater.from(context)
    private val items = RingtoneItem.generate(context)

    var listener: ((RingtoneItem) -> (Unit))? = null

    var selectedIndex = -1
        set(value) {
            if (value != selectedIndex) {
                val previous = selectedIndex
                field = value
                if (previous >= 0 && previous < itemCount) {
                    notifyItemRangeChanged(previous, 1)
                }
                if (value >= 0 && value < itemCount) {
                    notifyItemRangeChanged(value, 1)
                }
            }
        }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int)
            = ViewHolder(inflater.inflate(R.layout.row_ringtone, parent, false))

    override fun onBindViewHolder(holder: ViewHolder, position: Int)
            = holder.bind(position)

    override fun getItemCount() = items.size

    fun getItem(position: Int) = items[position]

    inner class ViewHolder(view: View) : RecyclerView.ViewHolder(view) {

        private val title by lazy { view.findViewById(R.id.title) as TextView }
        private val button by lazy { view.findViewById(R.id.button) as RadioButton }

        init {
            view.setOnClickListener {
                selectedIndex = adapterPosition
                listener?.invoke(getItem(selectedIndex))
            }
        }

        fun bind(position: Int) {
            title.text = getItem(position).entry
            button.isChecked = position == selectedIndex
        }
    }
}